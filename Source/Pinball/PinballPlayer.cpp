// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballPlayer.h"


int UPinballPlayer::GetPlayerNumber() const
{
	return PlayerNumber;
}

UMaterial* UPinballPlayer::GetMaterial() const
{
	return ActiveMaterial;
}
