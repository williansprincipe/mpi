// ot.cpp
// open mpi test
// wpf 20200509

#include<iostream>
#include<mpi.h>
#include<random>

int main(int argc, char* argv[])
{   int ret_val=0; // success
    MPI_Init(&argc,&argv);
    int selfRankWrtWorld;
    int worldCommSize;
    int nSlaves;
    MPI_Comm_rank(MPI_COMM_WORLD,&selfRankWrtWorld);
    MPI_Comm_size(MPI_COMM_WORLD,&worldCommSize);
    nSlaves=worldCommSize-1;

    std::random_device dev;
    std::mt19937 rng(dev());
    // distribution in range [1, 6]
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,6); 
    int rn;
    int orderNumber;

    orderNumber=0;

    // std::cout<<"selfRankWrtWorld="<<selfRankWrtWorld<<"\n";

    if (selfRankWrtWorld==0)
    {   std::cout << "[Master:] ot version 20200509085400.\n";
        std::cout << "[Master:] I entrust each of the "<<nSlaves
                  <<" slave processes to return me one random number.\n";
        for (int i=1;i<worldCommSize;++i)
        {   orderNumber=2*i;
            MPI_Send(&orderNumber,1,MPI_INT,i,1,MPI_COMM_WORLD);
            std::cout << "[Master:] To process "<<i
                      <<" I sent a message asking order number "
                      <<orderNumber<<".\n"; 
        }

        std::cout << "[Master:] Waiting...\n";

        for (int i=1;i<worldCommSize;++i)
        {   MPI_Recv(&rn,1,MPI_INT,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            std::cout << "[Master:] From process "<<i
                      <<" I received number "<<rn<<".\n"; 
        }
        MPI_Finalize();    
        std::cout << "[Master:] Program ot ended ";
        if (ret_val==0)
        {   std::cout << "successfuly."<<"\n";
        }
        else
        {
            std::cout << "with error code "<<ret_val<<".\n";
        }
        return ret_val;
    }

    std::cout << "[Process "<<selfRankWrtWorld
              << ":] I'm a slave process.\n";
    std::cout << "[Process "<<selfRankWrtWorld
              << ":] I'll listen to the first message from master...\n";
    MPI_Recv(&orderNumber,1,MPI_INT,0,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    std::cout << "[Process "<<selfRankWrtWorld
              << ":] First message from master says "<<orderNumber<<".\n";
    rn=dist6(rng);
    std::cout << "[Process " << selfRankWrtWorld 
              << ":] In response I'm sending "<<rn<<".\n";
    MPI_Send(&rn,1,MPI_INT,0,0,MPI_COMM_WORLD);
  
    MPI_Finalize();    
    std::cout << "[Process "<<selfRankWrtWorld<<":] Process "<<selfRankWrtWorld<<" ended ";
    if (ret_val==0)
    {   std::cout<<"successfuly."<<"\n";
    }
    else
    {   std::cout<<"with error code "<<ret_val<<".\n";
    }
    return ret_val;
}

//
// end of file ot.cpp
