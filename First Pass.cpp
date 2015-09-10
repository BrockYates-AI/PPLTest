// void main()
// {
//     auto gen = Generator();
// 
//     const auto foos = gen.CreateVec<Foo>();
//     const auto bars = gen.CreateVec<Bar>();
//     const auto bazs = gen.CreateVec<Baz>();
//     const auto quuxs = gen.CreateVec<Quux>();
// 
//     int64_t min;
//     int64_t max;
// 
//     const auto elapsed = time_call([&]()
//     {
//         const auto minStarts =
//         {
//             std::min_element(foos.begin(), foos.end(), StartLessThan())->GetInterval().GetStart(),
//             std::min_element(bars.begin(), bars.end(), StartLessThan())->GetInterval().GetStart(),
//             std::min_element(bazs.begin(), bazs.end(), StartLessThan())->GetInterval().GetStart(),
//             std::min_element(quuxs.begin(), quuxs.end(), StartLessThan())->GetInterval().GetStart()
//         };
// 
//         const auto maxEnds =
//         {
//             std::max_element(foos.begin(), foos.end(), EndLessThan())->GetInterval().GetEnd(),
//             std::max_element(bars.begin(), bars.end(), EndLessThan())->GetInterval().GetEnd(),
//             std::max_element(bazs.begin(), bazs.end(), EndLessThan())->GetInterval().GetEnd(),
//             std::max_element(quuxs.begin(), quuxs.end(), EndLessThan())->GetInterval().GetEnd()
//         };
// 
//         min = *std::min_element(minStarts.begin(), minStarts.end());
//         max = *std::max_element(maxEnds.begin(), maxEnds.end());
//     });
// 
//     wcout << "min: " << min;
//     wcout << endl;
//     wcout << "max: " << max;
//     wcout << endl << endl;
//     wcout << "elapsed: " << elapsed << " ms";
// 
//     _getch();
// }
