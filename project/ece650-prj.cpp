#include <iostream>
#include <pthread.h>
#include "graph.h"
#include "inputFunctions.h"
#include "vertexCoverSolver.h"
#include <iomanip>
#include "outputFunctions.h"
#include <chrono>


Graph* g = nullptr;
bool cnfThreadExited = false;
std::vector<int> minVertexCoverCNF;
std::vector<int> minVertexCoverApprox1;
std::vector<int> minVertexCoverApprox2;
std::vector<double> duration_cnf_sat_total;
std::vector<double> duration_vc1_total;
std::vector<double> duration_vc2_total;
std::vector<double> approx_v1c;
std::vector<double> approx_v2c;
long long cnf_run_time, vc_1_run_time, vc_2_run_time;
int vertex_no;

#define DEBUG std::getenv("DEBUG") != NULL
// std::string output = "";

double calculateStandardDeviation(const std::vector<double>& data, double mean) {
    //double mean = calculateMean(data);
    double variance = 0.0;

    for (double value : data) {
        variance += pow(value - mean, 2);
    }

    double stdDev = sqrt(variance / data.size());
    return stdDev;
}

double calculateMean(const std::vector<double>& data) {
    double sum = 0.0;
    for (double value : data) {
        sum += value;
    }
    return sum / data.size();
}

void* cnfSatThread(void* arg) {
    //std::cout << "Thread 1" << std::endl;
    // VertexCoverSolver* solver = static_cast<VertexCoverSolver*>(arg);
    clock_t start, finish;
    double duration;
    //auto start = std::chrono::high_resolution_clock::now();
    start = clock();
    //auto start = std::chrono::high_resolution_clock::now();
    
    cnfThreadExited = false;
    VertexCoverSolver vcs(g);
    minVertexCoverCNF = vcs.findMinVertexCover();
    finish = clock();
 
    duration = (double)((finish - start)*1000000) / CLOCKS_PER_SEC;
    // push the durations here
    
    //auto elapsed = std::chrono::high_resolution_clock::now() - start;
    //vc_1_run_time = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    vc_1_run_time = duration;

    //auto elapsed = std::chrono::high_resolution_clock::now() - start;
    //cnf_run_time = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    duration_cnf_sat_total.push_back(cnf_run_time);
    cnfThreadExited = true;
    return nullptr;
}

void* printVertexCover1Thread(void* arg) {
   // std::cout << "Thread 2"<<std::endl;
   clock_t start, finish;
    double duration;
    //auto start = std::chrono::high_resolution_clock::now();
    start = clock();
    minVertexCoverApprox1 = g->approximateMinimumVertexCover1();

    finish = clock();
 
    duration = (double)((finish - start)*1000000) / CLOCKS_PER_SEC;
    // push the durations here
    
    //auto elapsed = std::chrono::high_resolution_clock::now() - start;
    //vc_1_run_time = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    vc_1_run_time = duration;
    duration_vc1_total.push_back(vc_1_run_time);
    return nullptr;
}

void* printVertexCover2Thread(void* arg) {
    //std::cout << "Thread 3"<<std::endl;
    clock_t start, finish;
    double duration;
    //auto start = std::chrono::high_resolution_clock::now();
    start = clock();
    //auto start = std::chrono::high_resolution_clock::now();
    minVertexCoverApprox2 = g->approximateMinimumVertexCover2();
    finish = clock();
 
    duration = (double)((finish - start)*1000000) / CLOCKS_PER_SEC;
    vc_2_run_time = duration;
    //auto elapsed = std::chrono::high_resolution_clock::now() - start;
    //vc_2_run_time = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    duration_vc2_total.push_back(vc_2_run_time);
    return nullptr;
}

clockid_t get_cpu_clockid_for_thread(pthread_t thread_id)
{
    clockid_t cid;
    if (pthread_getcpuclockid(thread_id, &cid))
    {
        std::cerr << "Error: can't get the cpu clock" << std::endl;
    }
    return cid;
}
void thread_timeout_watcher(pthread_t thread_id, int time_limit)
{
    clockid_t cid = get_cpu_clockid_for_thread(thread_id);
    struct timespec tspec;

    while (!cnfThreadExited)
    {
        clock_gettime(cid, &tspec);
        if (tspec.tv_sec > time_limit)
        {
            VertexCoverSolver::cnf_timeout_flag = true;
            cnfThreadExited = true;
        }
    }
}

void threaded_vc() {
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    // // struct Thread_Args *args_vc1 = (struct Thread_Args*) malloc(sizeof(*args_vc1));


    pthread_create(&thread1, NULL, &cnfSatThread, nullptr);
    pthread_create(&thread2, NULL, &printVertexCover1Thread, nullptr);
    pthread_create(&thread3, NULL, &printVertexCover2Thread, nullptr);

    thread_timeout_watcher(thread1, 120);

    // // Timeout function for CNF-SAT-VC approach (Check "Formal DEBUG" pdf on Github repository for more information)
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 120;

    // // Awaiting thread termination
    
    
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_timedjoin_np(thread1, NULL, &ts);
    

// struct timespec ts;
// clock_gettime(CLOCK_REALTIME, &ts);
// ts.tv_sec += 10;

// // Awaiting thread termination with timeout
// while (1) {
//         int result1 = pthread_join(thread1, NULL);
//         int result2 = pthread_join(thread2, NULL);
//         int result3 = pthread_join(thread3, NULL);

//     if (result1 == 0 && result2 == 0 && result3 == 0) {
//         // All threads have terminated
//         break;
//     }

//     /// Check timeout
//         struct timespec current_time;
//         clock_gettime(CLOCK_REALTIME, &current_time);
//         if (current_time.tv_sec >= ts.tv_sec) {
//             // Timeout reached
//             exit(0);
//         }

//         // Sleep for a short duration before checking again
//         usleep(10000);  // 10 milliseconds
// }

//     if (!VertexCoverSolver::cnf_timeout_flag)
//     {
//         printMinimumVertexCover("CNF-SAT-VC: ", minVertexCoverCNF);
//     }
//     else
//     {
//         std::cout << "CNF-SAT-VC: timeout" << std::endl;
//         VertexCoverSolver::cnf_timeout_flag = false;
//     }

//     printMinimumVertexCover("APPROX-VC-1: ", minVertexCoverApprox1);
//     printMinimumVertexCover("APPROX-VC-2: ", minVertexCoverApprox2);

//     std::cout << "CNF-SAT-VC Runtime: " << cnf_run_time << " microseconds" << std::endl;
//    std::cout << "APPROX-VC-1 Runtime: " << vc_1_run_time << " microseconds" << std::endl;
//    std::cout << "APPROX-VC-2 Runtime: " << vc_2_run_time << " microseconds" << std::endl;
    // std::cout << output << std::flush;
    //std::cout << Graph::output_v1 << std::flush;
    // std::cout << std::endl;

approx_v1c.push_back(Graph::approx_1_size / VertexCoverSolver::cnf_size);
approx_v2c.push_back(Graph::approx_2_size / VertexCoverSolver::cnf_size);
   

   return;
}

int main() {
    while (!std::cin.eof()) {
        std::string line;

        while (std::getline(std::cin, line)) {
            // if cmd length is less than 2 characters
            if (line.length() <= 2) {
                std::cerr << "Error: Command length should be minimum 3 characters\n";
            }
            try {
                // Check cmd cases
                if (line[0] == 'V') {
                    readVertixNumberInput(g, line);
                    vertex_no = g->getVertixNumber();
                }
                else if (line[0] == 'E')
                {
                    if (g == nullptr) {
                        std::cerr << "Error: Graph not yet defined.\n";
                    } else if (!(g->getEdgeCommandGiven())) {
                        if (readEdgeInput(g, line)) {
                            threaded_vc();
                        }
                    }
                }
                else
                {
                    std::cerr << "Error: unknown command\n";
                }
            } catch (...) {
                std::cerr << "Error: Some error occurred during the parsing of the given command.\n";
            }
        }
    }
    std::cout << "Vertex no" << vertex_no << std::endl;
    // std::cout << "Duration array for CNF ";
    // for (double element : duration_cnf_sat_total)
    // {
    //     temp += element;
    //     std::cout << element << ", ";
    // }
    // std::cout << std::endl;
    //double mean = calculateMean(duration_cnf_sat_total);
    //std::cout << "Mean of CNF:" << mean << std::endl;
    //std::cout << "Std Deviation CNF:" <<calculateStandardDeviation(duration_cnf_sat_total, mean) <<std::endl;
    //temp = 0;

    // std::cout << "Duration array for VC1 ";
    // for (double element1 : duration_vc1_total)
    // {
    //     temp += element1;
    //     std::cout << element1<< ", ";
    // }
    //std::cout << std::endl;
    //double mean = calculateMean(duration_vc1_total);
    // std::cout << "Mean of VC1:" << mean << std::endl;
    // std::cout << "Std Deviation VC1:" <<calculateStandardDeviation(duration_vc1_total, mean) <<std::endl;
    // //temp = 0;
    // // //std::cout << "Duration array for VC2 ";
    // // for (double element2 : duration_vc2_total)
    // // {
    // //     temp += element2;
    // //     std::cout << element2<< ", ";
    // // }
    // // std::cout << std::endl;
    // mean = calculateMean(duration_vc2_total);
    // std::cout << "Mean of VC2:" << mean << std::endl;
    // std::cout << "Std Deviation VC2:" <<calculateStandardDeviation(duration_vc2_total, mean) <<std::endl;
    
    

double mean = calculateMean(approx_v1c);
std::cout << "Mean of approx of VC1:" << std::fixed << std::setprecision(2)<< mean<< std::endl;
std::cout << "Std Deviation approx of VC1:" <<std::fixed << std::setprecision(2)<< calculateStandardDeviation(approx_v1c, mean) <<std::endl;
mean = calculateMean(approx_v2c);
std::cout << "Mean of approx of VC2:" << std::fixed << std::setprecision(2)<< mean<< std::endl;
std::cout << "Std Deviation approx of VC2:" <<std::fixed << std::setprecision(2)<< calculateStandardDeviation(approx_v2c, mean) <<std::endl;

    

    return 0;
}
