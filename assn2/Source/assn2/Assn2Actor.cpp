// Fill out your copyright notice in the Description page of Project Settings.
#include "Assn2Actor.h"

//Creating structure to bypass ue4 lack of 2d arrays.
//This is an array of structs containing an array of ints
//I had some issues when part of this was in the header file so I just did it in here.
struct myVisited{
    bool myY[5];
};

struct myVisited visitedArray[5];

// Sets default values
AAssn2Actor::AAssn2Actor()
{   
    //created USceneComponent for Actor Placement
    const bool bTransient = true;
    USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"), bTransient);
    check(SceneComponent != nullptr);
    this->RootComponent = SceneComponent;
     
    //created instanced static mesh component
    StaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("MeshComponent"));
    StaticMeshComponent->SetupAttachment(RootComponent);
}

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

    //commented out loop for printing to log for debugging purposes

    /*
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
    */
    
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

//creates and renders maze
void AAssn2Actor::BuildPath(int startCoordX, int startCoordY, FVector currentAbove,FVector currentBelow, 
    FVector currentLeft, FVector currentRight)
{
    //dummy vectors for parsing next adjacent coordinates
    FVector Above=   FVector();
    FVector Below =  FVector();
    FVector Left =  FVector();
    FVector Right=  FVector();

    
    //reinitialize every time you build
    ////array for 4 directions of left right up and down
    //1 = left, 2 = right, 3 = up, 4 = down
    int directions[4] = {1,2,3,4};
    
    //randomize directions vector every time using fisher yates
    shuffle(directions,4);
    
    //loop through directions in shuffled random order
    //1 = left, 2 = right, 3 = up, 4 = down
    for(int i = 0; i< 4; i++)
    {
        //commented out code for printing to log for debugging purposes

        /*
        UE_LOG(LogTemp, Warning, TEXT("I am trying the direction %d (reminder 1 = left 2 =right 3 = up 4 = down"), directions[i]);

        UE_LOG(LogTemp, Warning, TEXT("This is for the build path starting at coordinates (%d,%d)"), startCoordX, startCoordY);
        */

        if(allowedMove(directions[i], startCoordX, startCoordY))
        {
            //commented out code for printing to log for debugging purposes
            //UE_LOG(LogTemp, Warning, TEXT("Direction %d was allowed."), directions[i]);


            //Make next node visited, delete/breakthrough correct wall, and
            //recursively call build path with next node coordinates
            switch (directions[i])
            {
                case 1:
                {
                    //commented out code for printing to log for debugging purposes

                    /*
                    UE_LOG(LogTemp, Warning, TEXT("moving left"));

                    UE_LOG(LogTemp, Warning, TEXT("I am moving to coordinates (%d,%d)"), startCoordX+1, startCoordY);
                    */

                    //update visited
                    visitedArray[startCoordX+1].myY[startCoordY]=true;
                    
                    //update surrounding vectors
                    Above.Set(currentAbove.X+100, currentAbove.Y, currentAbove.Z);

                    Below.Set(currentBelow.X+100, currentBelow.Y, currentBelow.Z);

                    Left.Set(currentLeft.X+100, currentLeft.Y, currentLeft.Z);

                    Right.Set(currentRight.X+100, currentRight.Y, currentRight.Z);

                    //add to list of planes we are ignoring when creating instances
                    toRemove.Add(currentLeft);
                    
                    //continue recursion with new starting matrix cell and surrounding vectors
                    BuildPath(startCoordX+1,startCoordY, Above, Below, Left, Right);
                    break;
                }
                case 2:
                {
                    //commented out code for printing to log for debugging purposes
                    
                    /*
                    UE_LOG(LogTemp, Warning, TEXT("moving right"));

                    UE_LOG(LogTemp, Warning, TEXT("I am moving to coordinates (%d,%d)"), startCoordX-1, startCoordY);
                    */

                    //update visited
                    visitedArray[startCoordX-1].myY[startCoordY]=true;

                    //update surrounding vectors
                    Above.Set(currentAbove.X-100, currentAbove.Y, currentAbove.Z);

                    Below.Set(currentBelow.X-100, currentBelow.Y, currentBelow.Z);

                    Left.Set(currentLeft.X-100, currentLeft.Y, currentLeft.Z);

                    Right.Set(currentRight.X-100, currentRight.Y, currentRight.Z);

                    //add to list of planes we are ignoring when creating instances
                    toRemove.Add(currentRight);
                    
                    //continue recursion with new starting matrix cell and surrounding vectors
                    BuildPath(startCoordX-1,startCoordY, Above, Below, Left, Right);
                    break;
                }
                case 3:
                {

                    //commented out code for printing to log for debugging purposes

                    /*
                    UE_LOG(LogTemp, Warning, TEXT("moving up"));

                    UE_LOG(LogTemp, Warning, TEXT("I am moving to coordinates (%d,%d)"), startCoordX, startCoordY+1);
                    */
                    
                    //update visited 
                    visitedArray[startCoordX].myY[startCoordY+1]=true;

                    //update surrounding vectors
                    Above.Set(currentAbove.X, currentAbove.Y+100, currentAbove.Z);

                    Below.Set(currentBelow.X, currentBelow.Y+100, currentBelow.Z);

                    Left.Set(currentLeft.X, currentLeft.Y+100, currentLeft.Z);

                    Right.Set(currentRight.X, currentRight.Y+100, currentRight.Z);

                    //add to list of planes to ignore
                    toRemove.Add(currentAbove);
                    
                    //continue recursion with new starting matrix cell and surrounding vectors
                    BuildPath(startCoordX,startCoordY+1, Above, Below, Left, Right);
                    break;
                }
                case 4:
                {

                    //commented out code for printing to log for debugging purposes

                    /*
                    UE_LOG(LogTemp, Warning, TEXT("moving down"));

                    UE_LOG(LogTemp, Warning, TEXT("I am moving to coordinates (%d,%d)"), startCoordX, startCoordY-1);
                    */

                    //update visited
                    visitedArray[startCoordX].myY[startCoordY-1]=true;

                    //update surrounding vectors
                    Above.Set(currentAbove.X, currentAbove.Y-100, currentAbove.Z);

                    Below.Set(currentBelow.X, currentBelow.Y-100, currentBelow.Z);

                    Left.Set(currentLeft.X, currentLeft.Y-100,currentLeft.Z);

                    Right.Set(currentRight.X, currentRight.Y-100, currentRight.Z);

                    //add to list of vectors to ignore when constructing instanced static mesh components
                    toRemove.Add(currentBelow);
                    
                    //continue recursion with new starting matrix cell and surrounding vectors
                    BuildPath(startCoordX,startCoordY-1, Above, Below, Left, Right);
                    break;
                }
                    
            }
            
        }
        
    }
    //Otherwise, backtrack by returning recursively until maze is back to start.

    //commented out code for printing to log for debugging purposes
    //UE_LOG(LogTemp, Warning, TEXT("I am returning from the build path starting at coordinates (%d,%d) after exhausting all options"), startCoordX, startCoordY);
    
    return;
    
}

//On construction overriden function
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

    //commented out code for printing to log for debugging purposes

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
    

    //gets coordinates 0 to 4 for x and y starting value of matrix
    int randomX = myStream.RandHelper(5);

    int randomY = myStream.RandHelper(5);

    //Record starting point as visited. In my representation this is the bottom right and the directions for positive movement
    //in terms of x and y coordinates are up and to the left respectively. I used a nonstandard orientation that matched up with 
    //the order in which I build the grid via my for loop.
    visitedArray[randomX].myY[randomY] = true;

    //stores surrounding walls of starting point 
    FVector Above = FVector((randomX*100)-250, (randomY*100)-150, 100);

    FVector Below =  FVector((randomX*100)-250, (randomY*100)-250, 100);

    FVector Left =  FVector((randomX*100)-200, (randomY*100)-200, 100);

    FVector Right =  FVector((randomX*100)-300, (randomY*100)-200, 100);

    //Call recursive backtracking algorithm for breaking through grid to create maze
    BuildPath(randomX, randomY, Above, Below, Left, Right);

    //commented out code for printing to log for debugging purposes
    
    /*
    UE_LOG(LogTemp, Warning, TEXT("End of all recursions of build path"));

    UE_LOG(LogTemp, Warning, TEXT("I shall now print the final state of the visited 2d matrix to check if it is all true."));

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
    */

    //I am now using a for loop to create my 5x5 grid from the matrix array. 
    //I am ignoring any walls set to be ignored in my toRemove Tarray.
    for(int i = 1; i<6 ; i++)
    {
        for(int j =1; j <6; j++)
        {
            if(j== 1 )
            {
                //draw bottom-most wall. loops through i but not j

                if(!toRemove.Contains(FVector((i*100)-350,(j*100)-350, 100)))
                    StaticMeshComponent->AddInstance(FTransform(FRotator(90, 90, 0), FVector((i*100)-350,(j*100)-350, 100)));
            }
            
            if(i == 1)
            {
                //draw rightmost wall. loops through j but not i
                if(!toRemove.Contains(FVector((i*100)-400,(j*100)-300, 100)))
                    StaticMeshComponent->AddInstance(FTransform(FRotator(90, 0, 0), FVector((i*100)-400,(j*100)-300, 100)));
            }
            
            
            //draw top wall iteratively with each loop
            if(!toRemove.Contains(FVector((i*100)-350,(j*100)-250, 100)))
                StaticMeshComponent->AddInstance(FTransform(FRotator(90, 90, 0), FVector((i*100)-350,(j*100)-250, 100)));
            
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

    //reset list of planes to remove
    toRemove.Empty();   
    
}

