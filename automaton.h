#ifndef CHRONOS_AUTOMATON_H
#define CHRONOS_AUTOMATON_H

enum State {
    DEFAULT,
    SET_HOUR,
    SET_MINUTE
};

extern volatile enum State state;

#endif //CHRONOS_AUTOMATON_H
