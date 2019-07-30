#ifndef COMM_UTILS_H
#define COMM_UTILS_H

const int MESSAGE_ACK = 1;

const int MESSAGE_SCORE = 11;
const int MESSAGE_DEATH = 12;

const int MESSAGE_START_GAME = 21;
const int MESSAGE_END_GAME = 22;

/**
 * Will try to get message from specific buffer
 * \return 0 if there is no message, otherwise message code
 */
int get_message(char *buf);

/**
 * \return Returns 1 if there is message to be read and 0 if there is not
 */
int check_for_message(char *buf);

/**
 * If message was read an processed then this function will clear the buffer
 */
void clear_message(char *buf); 

#endif
