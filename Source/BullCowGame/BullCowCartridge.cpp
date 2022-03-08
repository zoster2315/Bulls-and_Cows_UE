// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    ValidWords = GetValidWords(Words);
    SetupGame();
    WelcomePlayer();
}

void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
        WelcomePlayer();
        return;
    }

    ProcessGuess(Input);

    // Проверить изограмма ли слово
    // Проверить количество символов
    // Уменьшить жизни
}

void UBullCowCartridge::SetupGame()
{
    int32 Index = FMath::RandRange(0, ValidWords.Num() - 1);
    HiddenWord = ValidWords[Index];
    Lives = HiddenWord.Len();
    bGameOver = false;
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("You have no lives left!"));
    PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
    PrintLine(TEXT("\nPress Enter to restart"));
}

void UBullCowCartridge::WelcomePlayer()
{
    PrintLine(TEXT("Welcome to Bull Cows game!"));
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("The hidden word is %s."), *HiddenWord); // Debug line
    PrintLine(TEXT("You have %i lives"), Lives);
    PrintLine(TEXT("Press Enter to continue..."));
}

void UBullCowCartridge::ProcessGuess(const FString &Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine("You have won!");
        EndGame();
    }
    else
    {
        Lives--;

        if (Lives > 0)
            PrintLine(TEXT("You have %i lives"), Lives);
        else
        {
            PrintLine(TEXT("You lose. Try again..."));
            EndGame();
        }

        if (Guess.Len() != HiddenWord.Len())
        {
            PrintLine(TEXT("The Hidden Word is %i characters long, try again"), HiddenWord.Len());
        }

        if (!IsIsogram(Guess))
        {
            PrintLine(TEXT("No repeting letters, guess again"));
        }

        FBullCowCount Count = GetBullCows(Guess);
        PrintLine(TEXT("Bulls - %i, Cows - %i."), Count.Bulls, Count.Cows);
    }
}

bool UBullCowCartridge::IsIsogram(const FString &Guess) const
{
    for (int32 i = 0; i < Guess.Len(); i++)
        for (int32 j = i + 1; j < Guess.Len(); j++)
            if (Guess[i] == Guess[j])
                return false;
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString> &WordList) const
{
    TArray<FString> ValidWords;
    for (FString Word : WordList)
        if (IsIsogram(Word) && Word.Len() >= 4 && Word.Len() <= 8)
            ValidWords.Emplace(Word);
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString &Guess) const
{
    FBullCowCount Count;
    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == HiddenWord[i])
        {
            Count.Bulls++;
            continue;
        }
        for (int32 j = 0; j < HiddenWord.Len(); j++)
            if (Guess[i] == HiddenWord[j])
            {
                Count.Cows++;
                break;
            }
    }
    return Count;
}