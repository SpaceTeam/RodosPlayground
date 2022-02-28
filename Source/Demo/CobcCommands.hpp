#pragma once


#include <rodos.h>

#include <etl/string.h>


namespace rpg
{
// ETL Strings with internal buffer -> no null termination needed
constexpr auto cmdLengthEtl = 3;
constexpr auto maxDataLengthEtl = 64;


void TurnEduOn();
void TurnEduOff();
void SendResetCounter(const etl::string<cmdLengthEtl> & command);
void SendTemperature(const etl::string<cmdLengthEtl> & command);
void SendImage(const etl::string<cmdLengthEtl> & command);
}