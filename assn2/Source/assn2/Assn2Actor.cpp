// Fill out your copyright notice in the Description page of Project Settings.
#ifdef __clang__
#pragma clang optimize off
#else
#pragma optimize("", off)
#endif


// define a print message function to print to screen
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT(text), fstring))

#include "Assn2Actor.h"

//DEFINE_LOG_CATEGORY(genLog);

//stream for random variable based on seed
FRandomStream myStream;

//creating structure to bypass ue4 lack of 2d arrays.
//this is an array of structs containing an array of ints
struct myVisited{
    bool myY[5];
};

struct myVisited visitedArray[5];

//data structure for keeping adjacency list for each cell of matrix. 25 * 4
//basically keep order that instances were added in so as to later be able to remove them
//FVector4 adjacencyList[25];

//list of added vectors for transform in order as they are added to list of instances
//TArray<FVector2D> transformList;


// Sets default values
AAssn2Actor::AAssn2Actor()
{

    //initialize list of walls to remove. wait maybe I don't need a list initializer
    //toRemove=TArray();


    //2d arrays dont work in ue4 so delete this later
    //Creating structure for holding 2d array of visited nodes.
    //visited = new bool*[5];
    //create array of arrays/ 2nd dimension
    //for(int i = 0; i < 5; i++)
    // {
    //   visited[i] = new bool [5];
    //}
    //initialize all to false
    
    //for(int a = 0 ; a < 5; a++)
    //{
    //    for(int b = 0; b<5; b++)
    //    {
    //        visitedArray[a].myY[b] = false;
    //    }
    //}
    
    /*
    //For array number/5 = column array number % 5 = row
    //adjacency order is the vector in order bottom, right, top, left
    adjacencyList[0] = FVector4(0,1,2,3);
    adjacencyList[1] = FVector4(2,4,5,6);
    adjacencyList[2] = FVector4(5,7,8,9);
    adjacencyList[3] = FVector4(8,10,11,12);
    adjacencyList[4] = FVector4(11,13,14,15);
    adjacencyList[5] = FVector4(16,3,17,18);
    adjacencyList[6] = FVector4(17,6,19,20);
    adjacencyList[7] = FVector4(19,9,21,22);
    adjacencyList[8] = FVector4(21,12,23,24);
    adjacencyList[9] = FVector4(23,15,25,26);
    adjacencyList[10] = FVector4(27,18,28,29);
    adjacencyList[11] = FVector4(28,20,30,31);
    adjacencyList[12] = FVector4(30,22,32,33);
    adjacencyList[13] = FVector4(32,24,34,35);
    adjacencyList[14] = FVector4(34,26,36,37);
    adjacencyList[15] = FVector4(38,29,39,40);
    adjacencyList[16] = FVector4(39,31,41,42);
    adjacencyList[17] = FVector4(41,33,43,44);
    adjacencyList[18] = FVector4(43,35,45,46);
    adjacencyList[19] = FVector4(45,37,47,48);
    adjacencyList[20] = FVector4(49,40,50,51);
    adjacencyList[21] = FVector4(50,42,52,53);
    adjacencyList[22] = FVector4(52,44,54,55);
    adjacencyList[23] = FVector4(54,46,56,57);
    adjacencyList[24] = FVector4(56,48,58,59);
    */
    //Stores x and y coordinates from uinstancedstaticmeshcomponent instance for current matrix
    //box's 4 plane instances which are above, below, left, and right of it.
    //These are the transforms coordinates for x and y of said planes.
    //This data is used for deletion of plane for breakthrough to next part of maze.
    //Set to the 0,0 bottom right coordinates by default.
    //Then changed based on random start position. Then offset for every move.
    //Above= FVector2D(-250,-150);
    //Below = FVector2D(-250,-250);
    //Left = FVector2D(-200,-200);
    //Right = FVector2D(-300,-200);
    
    //created USceneComponent for Actor Placement
    const bool bTransient = true;
    USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"), bTransient);
    check(SceneComponent != nullptr);
    this->RootComponent = SceneComponent;
    
    
    //created instanced static mesh component
    StaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("MeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);

    StaticMeshComponent->SetGenerateOverlapEvents(true);


   // sphereAbove = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));

   // sphereBelow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));

    //sphereLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));

    //sphereRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
    
}

//make these static?
//do I even need them ever?
//I am arbitrarily setting my grid to be 5 x 5
//int mazeLength = 5;
//int mazeWidth = 5;
//int mazeSize= mazeLength * mazeWidth;

//
//make this static?
//initialize matrix of visited nodes to be false
//bool visited [5][5] = {{false}};

//array for 4 directions of left right up and down
//1 = left, 2 = right, 3 = up, 4 = down

//int directions[4] = {1,2,3,4};


//going to always try 0,0

//variables for holding starting position
//int startX;
//int startY;

//function for swapping array elements around. helper function for fisher yates


void AAssn2Actor::swap (int *swapOne, int * swapTwo)
{
    int temp = *swapOne;
    *swapOne = *swapTwo;
    *swapTwo = temp;
    
}


//function for removing node and updating adjacency matrix
//void AAssn2Actor::remove(int removed)
//{

    /*
    bool boolRemoved = false;
    //the following is some code for adjusting adjacency list since lists/arrays shift upon removal
    //int removed = 1;
    //bool foundRemoved = false;
    
    for(int i = 0; i<24; i++)
    {
        if(boolRemoved)
        {
            float tempX = (adjacencyList[i].X-1);
            float tempY = (adjacencyList[i].Y-1);
            float tempZ = (adjacencyList[i].Z-1);
            float tempW = (adjacencyList[i].W-1);

            adjacencyList[i].Set(tempX,tempY,tempZ,tempW );
        }

        else if(adjacencyList[i].X==removed)
        {
            //dummy variable
            float tempX = -1;
            //decrement all subsequent list members by one from now on
            float tempY = (adjacencyList[i].Y-1);
            float tempZ = (adjacencyList[i].Z-1);
            float tempW = (adjacencyList[i].W-1);

            adjacencyList[i].Set(tempX,tempY,tempZ,tempW );
            //dummy variable
            boolRemoved = true;
            removed = -1;
        }
        else if(adjacencyList[i].Y==removed)
        {
            //dummy variable
            float tempX = adjacencyList[i].X;
            //decrement all subsequent list members by one from now on
            float tempY = -1;
            float tempZ = (adjacencyList[i].Z-1);
            float tempW = (adjacencyList[i].W-1);

            adjacencyList[i].Set(tempX,tempY,tempZ,tempW );
            //dummy variable
            boolRemoved = true;
            removed = -1;
        }
        else if(adjacencyList[i].Z==removed)
        {
            //dummy variable
            float tempX = adjacencyList[i].X;
            //decrement all subsequent list members by one from now on
            float tempY = adjacencyList[i].Y;
            float tempZ = -1;
            float tempW = (adjacencyList[i].W-1);

            adjacencyList[i].Set(tempX,tempY,tempZ,tempW );
            //dummy variable
            boolRemoved = true;
            removed = -1;
        }
        else if(adjacencyList[i].W==removed)
        {
            //dummy variable
            boolRemoved = true;
            removed = -1;
        }
    }

    */
//}

//function for shuffling array of these directions using fisher yates
//using this to shuffle maze directions


void AAssn2Actor::shuffle( int array[], int size)
{
    for(int i= size-1; i>0; i--)
    {
        int random = myStream.RandHelper(i+1);
        
        swap(&array[i], &array[random]);
    }

    UE_LOG(LogTemp, Warning, TEXT("The random fisher yates array is:"));
    //print("The random fisher yates array is: ");
    for(int j = 0; j < 4 ; j++)
    {
        //UE_LOG(genLog, Warning, TEXT("Index [%d] has value %d"), j, array[j]);
        //FString tempOne = FString::FromInt(j);
        //FString tempTwo= FString::FromInt(array[j]);

        UE_LOG(LogTemp, Warning, TEXT("Index [%d] has value %d"), j, array[j]);

        //printFString("Index [%s]:", *tempOne);
        //printFString("value:  %s", *tempTwo);
    }
    
}


//checks if move is allowed
bool AAssn2Actor::allowedMove(int move, int startCoordX, int startCoordY)
{
    switch(move)
    {
        case 1:
            if(((startCoordX+1)<=4) && !(visitedArray[startCoordX+1].myY[startCoordY]))
            {
                return true;
            }
            return false;
            break;
            
        case 2:
            if(((startCoordX-1)>=0) && !(visitedArray[startCoordX-1].myY[startCoordY]))
            {
                return true;
            }
            return false;
            break;
            
        case 3:
            if(((startCoordY+1)<=4) && !(visitedArray[startCoordX].myY[startCoordY+1]))
            {
                return true;
            }
            return false;
            break;
            
        case 4:
            if(((startCoordY-1)>=0) && !(visitedArray[startCoordX].myY[startCoordY-1]))
            {
                return true;
            }
            return false;
            break;
            
    }
    
    return false;
}

/*
//find instance with appropriate transform for removal
int AAssn2Actor::FindPlane(FVector remove)
{



    TArray<int32> foundOverlaps = StaticMeshComponent->GetInstancesOverlappingSphere(remove,100, false);

    if(foundOverlaps.Num()>0)
        return foundOverlaps.Pop(false);
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Some kind of error with finding overlap with x = %f, y= %f z=%f "),remove.X, remove.Y, remove.Z);
        return -1;
    }
}
*/


//creates and renders maze
void AAssn2Actor::BuildPath(int startCoordX, int startCoordY, FVector currentAbove,FVector currentBelow, 
    FVector currentLeft, FVector currentRight)
{



    
    //printFString("This is build path starting at coordinates (%s,%s)", startCoordX, startCoordY);


    //print("I shall now print the current state of the visited 2d matrix");

/*    for(int q = 0; q<5; q++)
    {
        for(int w= 0; w <5; w++)
        {
            if(visitedArray[q].myY[w] == false)
            {

                UE_LOG(LogTemp, Warning, TEXT("Visited at coordinates [%d][%d] is false"), q, w);
                //FString tempOne = FString::FromInt(q);
                //FString tempTwo= FString::FromInt(w);
                //printFString("Index1 [%s]", *tempOne);
                //printFString("Index2 [%s] is false.", *tempTwo);
            }
                // UE_LOG(genLog, Warning, TEXT("Visited at coordinates [%d][%d] visited is false", q, w);
            else
            {

                UE_LOG(LogTemp, Warning, TEXT("Visited at coordinates [%d][%d] is true"), q, w);
                //FString tempOne = FString::FromInt(q);
                //FString tempTwo= FString::FromInt(w);
                //printFString("Index1 [%s]", *tempOne);
                //printFString("Index2 [%s] is true.", *tempTwo);
            }
                //UE_LOG(genLog, Warning, TEXT("Visited at coordinates [%d][%d] visited is false", q, w);
        }
    }
    //print("end of visited loop");
    UE_LOG(LogTemp, Warning, TEXT("end of visited loop"));

    */

    //dummy vector for parsing next adjacent coordinates
    FVector Above=   FVector();
    FVector Below =  FVector();
    FVector Left =  FVector();
    FVector Right=  FVector();

    
    //reinitialize every time you build
    ////array for 4 directions of left right up and down
    //1 = left, 2 = right, 3 = up, 4 = down
    int directions[4] = {1,2,3,4};

    //will later be used to store the plane we will be removing in the grid
    //int forRemoval = -1;

    //int numberInAdjacency = -1;
    
    //randomize directions vector every time using fisher yates
    shuffle(directions,4);
    
    //loop through directions in shuffled random order
    //1 = left, 2 = right, 3 = up, 4 = down
    for(int i = 0; i< 4; i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("I am trying the direction %d (reminder 1 = left 2 =right 3 = up 4 = down"), directions[i]);

        UE_LOG(LogTemp, Warning, TEXT("This is for the build path starting at coordinates (%d,%d)"), startCoordX, startCoordY);

        //FString tempOne = FString::FromInt(directions[i]);
        //printFString("I am trying the direction %s. (reminder 1 = left 2 =right 3 = up 4 = down",*tempOne);


        if(allowedMove(directions[i], startCoordX, startCoordY))
        {
            //FString tempTwo= FString::FromInt(directions[i]);
            //printFString("Direction %s was allowed", *tempTwo);
             UE_LOG(LogTemp, Warning, TEXT("Direction %d was allowed."), directions[i]);


            //Make next node visited, delete/breakthrough correct wall, and
            //recursively call build path with next node coordinates
            switch (directions[i])
            {
                case 1:
                {

                    UE_LOG(LogTemp, Warning, TEXT("moving left"));

                    UE_LOG(LogTemp, Warning, TEXT("I am moving to coordinates (%d,%d)"), startCoordX+1, startCoordY);
                    //print("moving left");

                    //update visited
                    visitedArray[startCoordX+1].myY[startCoordY]=true;
                    
                    //update surrounding vectors
                    Above.Set(currentAbove.X+100, currentAbove.Y, currentAbove.Z);

                    Below.Set(currentBelow.X+100, currentBelow.Y, currentBelow.Z);

                    Left.Set(currentLeft.X+100, currentLeft.Y, currentLeft.Z);

                    Right.Set(currentRight.X+100, currentRight.Y, currentRight.Z);


                    toRemove.Add(currentLeft);
                    //find which plane to remove
                    //int removal = FindPlane(currentLeft);
                    //remove it from my list of transforms
                    //transformList.RemoveAt(removal);
                    //remove it from my instances
                    //StaticMeshComponent->RemoveInstance(removal);

                    //find coordinate from adjacency list, remove correct plane, 
                    //and adjust adjacency matrix
                    //numberInAdjacency = (startCoordX*5)+(startCoordY);

                    //StaticMeshComponent->RemoveInstance(adjacencyList[numberInAdjacency].W);

                    //remove(adjacencyList[numberInAdjacency].W);
                    
                    //continue recursion with new starting matrix cell and surrounding planes
                    BuildPath(startCoordX+1,startCoordY, Above, Below, Left, Right);
                    break;
                }
                case 2:
                {

                    UE_LOG(LogTemp, Warning, TEXT("moving right"));

                    UE_LOG(LogTemp, Warning, TEXT("I am moving to coordinates (%d,%d)"), startCoordX-1, startCoordY);
                    //print("moving right");

                    visitedArray[startCoordX-1].myY[startCoordY]=true;

                    //update surrounding vectors
                    Above.Set(currentAbove.X-100, currentAbove.Y, currentAbove.Z);

                    Below.Set(currentBelow.X-100, currentBelow.Y, currentBelow.Z);

                    Left.Set(currentLeft.X-100, currentLeft.Y, currentLeft.Z);

                    Right.Set(currentRight.X-100, currentRight.Y, currentRight.Z);

                    
                    toRemove.Add(currentRight);
                    //find which plane to remove
                    //int removal = FindPlane(currentRight);
                    //remove it from my list of transforms
                    //transformList.RemoveAt(removal);
                    //remove it from my instances
                    //StaticMeshComponent->RemoveInstance(removal);

                    //find coordinate from adjacency list, remove correct plane, 
                    //and adjust adjacency matrix
                    //numberInAdjacency = (startCoordX*5)+(startCoordY);

                    //StaticMeshComponent->RemoveInstance(adjacencyList[numberInAdjacency].Y);

                    //remove(adjacencyList[numberInAdjacency].Y);
                    
                    //continue recursion with new starting matrix cell and surrounding planes
                    BuildPath(startCoordX-1,startCoordY, Above, Below, Left, Right);
                    break;
                }
                case 3:
                {


                    UE_LOG(LogTemp, Warning, TEXT("moving up"));

                    UE_LOG(LogTemp, Warning, TEXT("I am moving to coordinates (%d,%d)"), startCoordX, startCoordY+1);
                    //print("moving up");

                    visitedArray[startCoordX].myY[startCoordY+1]=true;

                    //update surrounding vectors
                    Above.Set(currentAbove.X, currentAbove.Y+100, currentAbove.Z);

                    Below.Set(currentBelow.X, currentBelow.Y+100, currentBelow.Z);

                    Left.Set(currentLeft.X, currentLeft.Y+100, currentLeft.Z);

                    Right.Set(currentRight.X, currentRight.Y+100, currentRight.Z);

                    toRemove.Add(currentAbove);
                    //find which plane to remove
                    //int removal = FindPlane(currentAbove);
                    //remove it from my list of transforms
                    //transformList.RemoveAt(removal);
                    //remove it from my instances
                    //StaticMeshComponent->RemoveInstance(removal);

                    //find coordinate from adjacency list, remove correct plane, 
                    //and adjust adjacency matrix
                    //numberInAdjacency = (startCoordX*5)+(startCoordY);

                    //StaticMeshComponent->RemoveInstance(adjacencyList[numberInAdjacency].Z);

                    //remove(adjacencyList[numberInAdjacency].Z);
                    
                    //continue recursion with new starting matrix cell and surrounding planes
                    BuildPath(startCoordX,startCoordY+1, Above, Below, Left, Right);
                    break;
                }
                case 4:
                {
                    UE_LOG(LogTemp, Warning, TEXT("moving down"));

                    UE_LOG(LogTemp, Warning, TEXT("I am moving to coordinates (%d,%d)"), startCoordX, startCoordY-1);
                    //print("moving down");

                    visitedArray[startCoordX].myY[startCoordY-1]=true;

                    //update surrounding vectors
                    Above.Set(currentAbove.X, currentAbove.Y-100, currentAbove.Z);

                    Below.Set(currentBelow.X, currentBelow.Y-100, currentBelow.Z);

                    Left.Set(currentLeft.X, currentLeft.Y-100,currentLeft.Z);

                    Right.Set(currentRight.X, currentRight.Y-100, currentRight.Z);

                    toRemove.Add(currentBelow);
                    //find which plane to remove
                    //int removal = FindPlane(currentBelow);
                    //remove it from my list of transforms
                    //transformList.RemoveAt(removal);
                    //remove it from my instances
                    //StaticMeshComponent->RemoveInstance(removal);
                    
                    //find coordinate from adjacency list, remove correct plane, 
                    //and adjust adjacency matrix
                    //numberInAdjacency = (startCoordX*5)+(startCoordY);

                    //StaticMeshComponent->RemoveInstance(adjacencyList[numberInAdjacency].X);

                    //remove(adjacencyList[numberInAdjacency].X);
                    
                    //continue recursion with new starting matrix cell and surrounding planes
                    BuildPath(startCoordX,startCoordY-1, Above, Below, Left, Right);
                    break;
                }
                    
            }
            
        }
        
    }
    //Otherwise, backtrack by returning recursively until maze is back to start.

    UE_LOG(LogTemp, Warning, TEXT("I am returning from the build path starting at coordinates (%d,%d) after exhausting all options"), startCoordX, startCoordY);
    return;
    
}


//This will hold the
//on construction overriden function
void AAssn2Actor::OnConstruction(const FTransform& transform)
{
    Super::OnConstruction(transform);
    
    //Initialize the random generator with seed
    myStream = FRandomStream(RandomSeed);
    
    //clearing instances to reset tarray of instances at start of onconstruction
    StaticMeshComponent->ClearInstances();
    
    
    
    //reinitialize visited matrix to false
    for(int a = 0 ; a < 5; a++)
    {
        for(int b = 0; b<5; b++)
        {
            visitedArray[a].myY[b] = false;
        }
    }

    /*
    UE_LOG(LogTemp, Warning, TEXT("I shall now print the initialized state of the visited 2d matrix to check if it is false."));
    //print("I shall now print the initialized state of the visited 2d matrix to check if it is false.");

    for(int q = 0; q<5; q++)
    {
        for(int w= 0; w <5; w++)
        {
            if(visitedArray[q].myY[w] == false)
            {

                UE_LOG(LogTemp, Warning, TEXT("Visited at coordinates [%d][%d] is false"), q, w);
                //FString tempOne = FString::FromInt(q);
                //FString tempTwo= FString::FromInt(w);
                //printFString("Index1 [%s]", *tempOne);
                //printFString("Index2 [%s] is false.", *tempTwo);
            }
                // UE_LOG(genLog, Warning, TEXT("Visited at coordinates [%d][%d] visited is false", q, w);
            else
            {

                UE_LOG(LogTemp, Warning, TEXT("Visited at coordinates [%d][%d] is true"), q, w);
                //FString tempOne = FString::FromInt(q);
                //FString tempTwo= FString::FromInt(w);
                //printFString("Index1 [%s]", *tempOne);
                //printFString("Index2 [%s] is true.", *tempTwo);
            }
                //UE_LOG(genLog, Warning, TEXT("Visited at coordinates [%d][%d] visited is false", q, w);
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("end of visited loop"));
    */
    //print("end of visited loop");

    //For array number/5 = column array number % 5 = row
    //adjacency order is the vector in order bottom, right, top, left
    /*
    adjacencyList[0] = FVector4(0,1,2,3);
    adjacencyList[1] = FVector4(2,4,5,6);
    adjacencyList[2] = FVector4(5,7,8,9);
    adjacencyList[3] = FVector4(8,10,11,12);
    adjacencyList[4] = FVector4(11,13,14,15);
    adjacencyList[5] = FVector4(16,3,17,18);
    adjacencyList[6] = FVector4(17,6,19,20);
    adjacencyList[7] = FVector4(19,9,21,22);
    adjacencyList[8] = FVector4(21,12,23,24);
    adjacencyList[9] = FVector4(23,15,25,26);
    adjacencyList[10] = FVector4(27,18,28,29);
    adjacencyList[11] = FVector4(28,20,30,31);
    adjacencyList[12] = FVector4(30,22,32,33);
    adjacencyList[13] = FVector4(32,24,34,35);
    adjacencyList[14] = FVector4(34,26,36,37);
    adjacencyList[15] = FVector4(38,29,39,40);
    adjacencyList[16] = FVector4(39,31,41,42);
    adjacencyList[17] = FVector4(41,33,43,44);
    adjacencyList[18] = FVector4(43,35,45,46);
    adjacencyList[19] = FVector4(45,37,47,48);
    adjacencyList[20] = FVector4(49,40,50,51);
    adjacencyList[21] = FVector4(50,42,52,53);
    adjacencyList[22] = FVector4(52,44,54,55);
    adjacencyList[23] = FVector4(54,46,56,57);
    adjacencyList[24] = FVector4(56,48,58,59);

    */

    //going to always start at 0
    
    
    //Random starting coordinates. In the matrix, (0,0) will be the bottom right corner.
    //X will represent displacement to the left while Y represents displacement upwards.
    //startX = myStream.RandHelper(mazeLength);
    //startY= myStream.RandHelper(mazeWidth);
    
    
    //not doing this. always starting 0, 0
    //Update coordinates of transform for planes above, below, left, right
    //of current starting point.
    //Above.X= (-250 + (startX*100));
    //Above.Y= (-150 + (startY*100));
    
    //Below.X= (-250 + (startX*100));
    //Below.Y= (-250 + (startY*100));
    
    //Left.X= (-200 + (startX*100));
    //Left.Y= (-200 + (startY*100));
    
    //Right.X= (-300 + (startX*100));
    //Right.Y= (-200 + (startY*100));
    
    
    

    

    //adjust adjacency list. don't need to bother to adjust for 58 since it is last node
    //remove(1);

    //maybe change this elsewhere?
    //make starting node visited
    //visitX[0].visitY[0]= true;
    visitedArray[0].myY[0] = true;

    //stores surrounding walls of starting point which is (0,0)
    FVector Above = FVector(-250, -150, 100);

    FVector Below =  FVector(-250, -250, 100);

    FVector Left =  FVector(-200, -200, 100);

    FVector Right =  FVector(-300, -200, 100);

    //Call recursive backtracking algorithm for breaking through grid to create maze
    BuildPath(0, 0, Above, Below, Left, Right);

    UE_LOG(LogTemp, Warning, TEXT("End of all recursions of build path"));

    UE_LOG(LogTemp, Warning, TEXT("I shall now print the final state of the visited 2d matrix to check if it is all true."));
    //print("I shall now print the initialized state of the visited 2d matrix to check if it is false.");

    for(int q = 0; q<5; q++)
    {
        for(int w= 0; w <5; w++)
        {
            if(visitedArray[q].myY[w] == false)
            {

                UE_LOG(LogTemp, Warning, TEXT("Visited at coordinates [%d][%d] is false"), q, w);
                //FString tempOne = FString::FromInt(q);
                //FString tempTwo= FString::FromInt(w);
                //printFString("Index1 [%s]", *tempOne);
                //printFString("Index2 [%s] is false.", *tempTwo);
            }
                // UE_LOG(genLog, Warning, TEXT("Visited at coordinates [%d][%d] visited is false", q, w);
            else
            {

                UE_LOG(LogTemp, Warning, TEXT("Visited at coordinates [%d][%d] is true"), q, w);
                //FString tempOne = FString::FromInt(q);
                //FString tempTwo= FString::FromInt(w);
                //printFString("Index1 [%s]", *tempOne);
                //printFString("Index2 [%s] is true.", *tempTwo);
            }
                //UE_LOG(genLog, Warning, TEXT("Visited at coordinates [%d][%d] visited is false", q, w);
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("end of last visited loop\n \n \n "));


    //I am now using a for loop to create my 5x5 grid from the matrix array
    //Later I will remove walls as necessary via recursive backtrack
    //Also storing list of maze wall (plane) transforms so as I remove instances of the mesh,
    // I can continue to track plane instances.
    for(int i = 1; i<6 ; i++)
    {
        for(int j =1; j <6; j++)
        {
            if(j== 1 )
            {
                //transformList.Emplace(FVector((i*100)-350,(j*100)-350, 100));
                //draw bottom-most wall. loops through i but not j

                if(!toRemove.Contains(FVector((i*100)-350,(j*100)-350, 100)))
                    StaticMeshComponent->AddInstance(FTransform(FRotator(90, 90, 0), FVector((i*100)-350,(j*100)-350, 100)));
            }
            
            if(i == 1)
            {
                //transformList.Emplace(FVector((i*100)-350,(j*100)-350, 100));
                //draw rightmost wall. loops through j but not i
                if(!toRemove.Contains(FVector((i*100)-400,(j*100)-300, 100)))
                    StaticMeshComponent->AddInstance(FTransform(FRotator(90, 0, 0), FVector((i*100)-400,(j*100)-300, 100)));
            }
            
            
            //transformList.Emplace(FVector((i*100)-350,(j*100)-350, 100));
            //draw top wall iteratively with each loop
            if(!toRemove.Contains(FVector((i*100)-350,(j*100)-250, 100)))
                StaticMeshComponent->AddInstance(FTransform(FRotator(90, 90, 0), FVector((i*100)-350,(j*100)-250, 100)));
            
            //transformList.Emplace(FVector((i*100)-350,(j*100)-350, 100));
            //draw left wall iteratively with each loop
            if(!toRemove.Contains(FVector((i*100)-300,(j*100)-300, 100)))
                StaticMeshComponent->AddInstance(FTransform(FRotator(90, 0, 0), FVector((i*100)-300,(j*100)-300, 100)));
            
        }
    }


    //Always make arbitrary entrances to maze on opposite corners.
    //Recursive backtracking creates inside of maze but entrance and exit
    //would be out of bounds breakthroughs.
    StaticMeshComponent->RemoveInstance(1);
    StaticMeshComponent->RemoveInstance(StaticMeshComponent->GetInstanceCount()-1);
    
    //using member variable to set mesh
    StaticMeshComponent->SetStaticMesh(ActorMesh);
    
    //using setmaterial and our member variable
    StaticMeshComponent->SetMaterial(0, ActorMaterial);
    
    //empty transform list at the end since we are resetting instances and the transformlist. 
    //transformList.Empty();
    
}

#ifdef __clang__
#pragma clang optimize on
#else
#pragma optimize("", on)
#endif

