#ifndef COBC_COMMANDS_HPP
#define COBC_COMMANDS_HPP

void turnEduOn(const size_t id);
void turnEduOff(const size_t id);
void sendResetCounter(const size_t id);
void sendTemperature(const size_t id);
void sendImage(const size_t id);
void unhandledCommand(const size_t id);

#endif //COBC_COMMANDS_HPP