erStart = clock::now();
    for (auto i : dataSet)
    {
        tree->add(i, i + 1);
    }
    for (auto i : dataSet)
    {
        tree->printPath(i);
        cout<<"iiii:" <<i<<" "<<tree->get(i)<<endl;
        break;
    }
    auto timerEnd = clock::now();
    auto spentTime = std::chrono::duration_cast<std::chrono::nanoseconds>(timerEnd - timerStart).count();
    cout<< "time 1: "<<spentTime;