// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroInfo.h"


void UHeroInfo::Serialize(FArchive& Ar) {
	Ar << HeroName;
	Ar << UnitId;

	int num = Exps.Num();
	bool empty = num == 0;
	Ar << num;

	/*auto Iter = Exps.CreateConstIterator();
	for (int Index = 0; Index < num; ++Index) {
		if (empty) {
			uint8 ExpType = 0;
			Ar << ExpType;

			FExpInfo expInfo;
			expInfo.Serialize(Ar);

			Exps.Add(expInfo);
		}
		else {
			uint8 ExpType = Iter->Key;
			Ar << ExpType;

			Iter->Value->Serialize(Ar);

			++Iter;
		}
	}*/

	Ar << EquipItems;
}