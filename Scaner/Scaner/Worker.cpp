#include "Worker.h"

using namespace System::Windows::Forms;
using namespace System::Threading;

void Worker::Init()
{
    rdr = new Reader();
    formPtr->addLogLine("Hello.\n");
    formPtr->addLogLine("Scanning initiated.\n");
    formPtr->addLogLine("Every 50 milliseconds map will be updated.\n");

    // initial read local player info
    rdr->UpdatePlayerInfo();
}

long Worker::getMSTime()
{
    using namespace std::chrono;
    auto time = system_clock::now();

    auto since_epoch = time.time_since_epoch();

    auto milis = duration_cast<milliseconds>(since_epoch);
    
    return long(milis.count());
}

void Worker::doWork()
{
    long lastTime = getMSTime();
    bool startmessage = true;
    while (1)
    {
        if (formPtr->Created)
        {
            // serious stuff
            if (startmessage)
            {
                Init();
                startmessage = false;
            }
            else
            {
                long diff = getMSTime() - lastTime; // time diff between loops
                lastTime = getMSTime(); // set lastTime to now
                if (updateTimer - diff <= 0)
                {
                    Tick();
                    updateTimer = 50; // 50 ms = 0.05s
                } else updateTimer -= diff;
            }
        }

    }
}

void Worker::Tick()
{
    // update player data(position/guid etc)    
    rdr->UpdatePlayerInfo();

    // gps stuff
    rdr->GetObjectsForMap(75);
    for (std::list<object>::const_iterator iter = rdr->list.begin(); iter != rdr->list.end(); ++iter)
    {
        object obj = (*iter);
        System::String ^name = gcnew System::String(obj.name.c_str());

        // calculate possition for map
        int x, y;
        rdr->PositionForMap(obj.x, obj.y, x, y);

        if (rdr->GetLocalGUID() == obj.guid && obj.type == 4)// local player is always in center of map that we'll draw
        {
            x = 0;
            y = 0;
        }

        switch (obj.type)
        {
        case 3: // unit
            formPtr->DrawDot(105 + x, 105 + y, 4, System::Drawing::Color::Red);
            break;
        case 4: // player
            formPtr->DrawDot(105 + x, 105 + y, 6, System::Drawing::Color::Green);
            break;
        case 5: //gobject
            formPtr->DrawDot(105 + x, 105 + y, 4, System::Drawing::Color::Blue);
            break;
        }
    }
    // now when we have image drawn in offscreenbuffer redraw it to onscreen buffer
    formPtr->RedrawMap();
}

Worker::Worker()
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    Thread ^t1 = gcnew Thread(gcnew ThreadStart(this, &Worker::doWork));
    Scaner::MyForm form(t1);
    formPtr = %form;
    
    Application::Run(%form);

    // set timer
    updateTimer = 500;
}
