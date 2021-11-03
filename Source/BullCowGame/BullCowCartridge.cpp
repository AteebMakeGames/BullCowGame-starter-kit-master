// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
// #include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // Checking PlayerGuess
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    // Welcoming the player
    PrintLine(TEXT("Hey! Welcome to the \"Bull Cows\" game!!"));
    
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)]; // Set the HiddenWord
    Lives = HiddenWord.Len() * 2; // Set Lives
    bGameOver = false;

    TArray<FString> FWord;
    TArray<FString> MWord;
    TArray<FString> LWord;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives."), Lives);
    // PrintLine(TEXT("The first and last characters are: %c"), *HiddenWord[0]);
    PrintLine(TEXT("Type in your guess and \npress enter to continue..."));
    
    for (int32 Index = 0; Index < HiddenWord.Len(); Index++)
    {
        if (Index == 0)
        {
            PrintLine(TEXT("The first character of the string is: %c."), HiddenWord[0]);
        }
        if (HiddenWord.Len() == 4)
        {
            PrintLine(TEXT("And the last Word is: %c."), HiddenWord[HiddenWord.Len() - 1]);
            break;            
        }
        if (HiddenWord.Len() == 5)
        {
            int32 RandInt = FMath::RandRange(1,3);
            PrintLine(TEXT("A middle character is: %c."), HiddenWord[RandInt]);
            PrintLine(TEXT("And the last Word is: %c."), HiddenWord[HiddenWord.Len() - 1]);
            break;            
        }
        if (HiddenWord.Len() == 6)
        {
            int32 RandInt = FMath::RandRange(1,4);
            PrintLine(TEXT("A middle character is: %c."), HiddenWord[RandInt]);
            PrintLine(TEXT("And the last Word is: %c."), HiddenWord[HiddenWord.Len() - 1]);
            break;            
        }
    }
}


void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if (Guess == HiddenWord)
        {
            PrintLine(TEXT("YOU HAVE WON!!"));
            EndGame();
            return;
        }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long"), HiddenWord.Len());
        PrintLine(TEXT("Sorry, try guessing again!\nYou currently have %i live(s)."), Lives);
        return;
    }

    // Check If Isogram 
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess agian!!"));
        return;
    }

    // Remove lives
    PrintLine(TEXT("LOST A LIFE!"));
    --Lives;

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have lost a life!\nYou currently have %i live(s).\n--- YOU LOST THE GAME ---"), Lives);
        PrintLine(TEXT("The Hidden Word was: %s"), *HiddenWord);
        EndGame();
        return;
    }

    // Show the player Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows."), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Guess again, you have %i lives left"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{

    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparision = Index + 1; Comparision < Word.Len(); Comparision++)
        {
            if (Word[Index] == Word[Comparision])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 6 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}
