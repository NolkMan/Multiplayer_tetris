#ifndef COMM_UTILS_H
#define COMM_UTILS_H

#define MESSAGE_ACK 1

#define MESSAGE_SCORE 11
#define MESSAGE_DEATH 12

#define MESSAGE_GAME_START 21
#define MESSAGE_GAME_END 22

/**
 * Will generate message coresponding to that code
 * \param code code for the message to be generated
 * \return NULL if memory could not be allocated, otherwise pointer to message string that needs to be manually freed
 */
char * generate_message(int code);

/**
 * Will try to get message from specific buffer
 * \return -1 on error and 0 if there is no message and otherwise message code
 */
int check_for_message(char *buf);

/**
 * If message was read an processed then this function will clear the buffer
 */
void clear_message(char *buf); 

#endif
