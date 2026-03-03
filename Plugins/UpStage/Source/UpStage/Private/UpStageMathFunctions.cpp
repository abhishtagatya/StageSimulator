// Fill out your copyright notice in the Description page of Project Settings.


#include "UpStageMathFunctions.h"

float UUpStageMathFunctions::Normalize(const float X, const float K)
{
	return X / (X + K);
}

float UUpStageMathFunctions::InverseRatio(const float X)
{
	if (X <= 0.f) return 0.f;
	return 1.0f - (1.0f / X);
}

float UUpStageMathFunctions::ClampedInverseRatio(const float X, const float Min, const float Max)
{
    return FMath::Clamp(InverseRatio(X), Min, Max);
}