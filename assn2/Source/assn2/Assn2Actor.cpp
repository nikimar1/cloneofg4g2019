// Fill out your copyright notice in the Description page of Project Settings.
#ifdef __clang__
#pragma clang optimize off
#else
#pragma optimize("", off)
#endif

#include "Assn2Actor.h"


// Sets default values
AAssn2Actor::AAssn2Actor()
{
    
    //created USceneComponent for Actor Placement
    const bool bTransient = true;
    USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"), bTransient);
    check(SceneComponent != nullptr);
    this->RootComponent = SceneComponent;
    
    
    //created static mesh component
    StaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("MeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);

}

//for forcing rerun of on construction via boolean check mark
UPROPERTY(EditAnywhere) bool Update;

//for random seed generation
UPROPERTY(EditAnywhere) int RandomSeed;


/*

//Creating a matrix node struct for holding all coordinates of 5x5 grid
//For some reason could not place this in header. Type was not recognized
typedef struct myMatrixNode
{
    
    //rotation
    float pitch;
    float roll;
    
    //translation
    float x;
    float y;
    
}myMatrixNode;

*/
/*
//Node struct for holding moves. To be used in array for storing order of moves.
typedef struct myMove
{
    
    //where in the matrix you move from
    int fromX;
    int fromY;
    
    //where in matrix you move to
    int toX;
    int toY;
    
    //direction to make it easier to build walls
    int direction;
    
}myMove;
*/

//Created array of matrix node strucs via initialization list
/*
 
 Note: starting from top left 0,0 point I am building left wall top wall right wall bottom wall,
 then building next walls cells in order top, left,
 
 */
/*
myMatrixNode matrixArray[60] =
{
    //top
    {90,0,50,0},
    
    //bottom
    {90,0,-50,0},
    
    //990
    {90,90,0,-50},{90,90,0,50},{90,0,-150,0},{90,90,-100,-50},{90,90,-100,50},{90,0,-250,0},{90,90,-200,-50},{90,90,-200,50},{90,0,150,0},{90,90,100,-50},{90,90,100,50},{90,0,250,0},{90,90,200,-50},{90,90,200,50},{90,0,50,-100},{90,0,-50,-100},{90,90,0,-150},{90,0,-150,-100},{90,90,-100,-150},{90,0,-250,-100},{90,90,-200,-150},{90,0,150,-100},{90,90,100,-150},{90,0,250,-100},{90,90,200,-150},{90,0,50,-200},{90,0,-50,-200},{90,90,0,-250},{90,0,-150,-200},{90,90,-100,-250},{90,0,-250,-200},{90,90,-200,-250},{90,0,150,-200},{90,90,100,-250},{90,0,250,-200},{90,90,200,-250},{90,0,50,100},{90,0,-50,100},{90,90,0,150},{90,0,-150,100},{90,90,-100,150},{90,0,-250,100},{90,90,-200,150},{90,0,150,100},{90,90,100,150},{90,0,250,100},{90,90,200,150},{90,0,50,200},{90,0,-50,200},{90,90,0,250},{90,0,-150,200},{90,90,-100,250},{90,0,-250,200},{90,90,-200,250},{90,0,150,200},{90,90,100,250},{90,0,250,200},{90,90,200,250}
 
};

*/

//I am arbitrarily setting my grid to be 5 x 5
int mazeLength = 5;
int mazeWidth = 5;
int mazeSize= mazeLength * mazeWidth;

//initialize matrix of visited nodes to be false
bool visited [5][5] = {{false}};


//initialize list of order of moves. array of 2d vectors
//TArray<myMove> moveList;

//array for 4 directions of left right up and down
//1 = left, 2 = right, 3 = up, 4 = down
int directions[4] = {1,2,3,4};

//Initialize the random generator with seed
FRandomStream myStream = FRandomStream(RandomSeed);

//variables for holding starting position
int startX;
int startY;

//these variables will hold last visited point and place in list of moves where one is.
//start with dummy variables
int lastX = -1;
int lastY = -1;

//function for swapping array elements around. helper function for fisher yates
void AAssn2Actor::swap (int *swapOne, int * swapTwo)
{
    int temp = *swapOne;
    *swapOne = *swapTwo;
    *swapTwo = temp;
    
}

//function for shuffling array of these directions using fisher yates
//using this to shuffle maze directions
void AAssn2Actor::shuffle( int array[], int size)
{
    for(int i= size-1; i>0; i--)
    {
        int random = myStream.RandHelper(i+1);
        
        swap(&array[i], &array[random]);
    }
    
}

//checks if move is allowed
bool AAssn2Actor::allowedMove(int move)
{
    switch(move)
    {
        case 1:
            if((startX-1)>=0 && !visited[startX-1][startY])
            {
                return true;
            }
            return false;
            break;
        
        case 2:
            if((startX+1)<=4 && !visited[startX+1][startY])
            {
                return true;
            }
            return false;
            break;
        
        case 3:
            if((startY-1)>=0 && !visited[startX][startY-1])
            {
                return true;
            }
            return false;
            break;
            
        case 4:
            if((startX+1)<=4 && !visited[startX][startY+1])
            {
                return true;
            }
            return false;
            break;
            
    }
    
    return false;
}

//creates and renders maze
void AAssn2Actor::BuildPath(int startCoordX, int startCoordY)
{
    
    
    shuffle(directions,4);
    
    //bool needBacktrack = false;
    
    for(int i = 0; i< 4; i++)
    {
        if(allowedMove(directions[i]))
        {
            //the move i has been selected so add it to list of moves
            //myMove *tempMove = new myMove;
            //&tempMove.direction = i;
            //tempMove
            //I stopped writing code here. write it later.
            
            //delete wall in correct direction
            
        }
        
    }
    
    //otherwise, backtrack
    while(!(lastX==startX && lastY==startY))
    {
        
    }
    
}

//This will hold the
//on construction overriden function
void AAssn2Actor::OnConstruction(const FTransform& transform)
{
    Super::OnConstruction(transform);
    
    //clearing instances to reset tarray of instances at start of onconstruction
    StaticMeshComponent->ClearInstances();
    
    //Random starting coordinates. In the matrix, (0,0) will be the top left corner.
    //X will represent displacement to the right while Y represents displacement downwards.
    startX = myStream.RandHelper(mazeLength-1);
    startY= myStream.RandHelper(mazeWidth-1);
    
    
    
    
    //I am now using a for loop to create my 5x5 grid from the matrix array
    //Later I will remove walls as necessary
    for(int i = 1; i<6 ; ++i)
    {
        for(int j =1; j <6; ++j)
        {
            //if first row then need top wall as well
            if(j== 1 )
            {
                //draw top
                StaticMeshComponent->AddInstance(FTransform(FRotator(90, 90, 0), FVector((i*100),(j*100), 100)));
            }
            //if first column need left wall as well
            if(i == 1)
            {
                //draw left
                StaticMeshComponent->AddInstance(FTransform(FRotator(90, 0, 0), FVector((i*100)-50,(j*100)+50, 100)));
            }
            
            
            //draw bottom
            StaticMeshComponent->AddInstance(FTransform(FRotator(90, 90, 0), FVector(i*100,(j*100)+100, 100)));
            
            //draw right
            StaticMeshComponent->AddInstance(FTransform(FRotator(90, 0, 0), FVector((i*100)+50,(j*100)+50, 100)));
            
        }
    }
    
    
    /*
    for(int i = 0; i <60; i++)
    {
        //rotation
        float pitch = matrixArray[i].pitch;
        float roll = matrixArray[i].roll;
        
        //translation
        float x = matrixArray[i].x;
        float y = matrixArray[i].y;
        StaticMeshComponent->AddInstance(FTransform(FRotator(pitch, roll, 0), FVector(x, y, 100)));
    }
    */
    
    
    //call this when I finish it but for now commenting out
    //BuildPath(startX, startY);
    
    //using member variable to set mesh
    StaticMeshComponent->SetStaticMesh(ActorMesh);
    
    //using setmaterial and our member variable
    StaticMeshComponent->SetMaterial(0, ActorMaterial);
    
   
    
}

#ifdef __clang__
#pragma clang optimize on
#else
#pragma optimize("", on)
#endif
