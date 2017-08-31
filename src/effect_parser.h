#ifndef EFFECT_PARSER_H
#define EFFECT_PARSER_H
#include <string>

class ConditionalEffect;
class ConditionalEffects;

class EffectParser {
public:
    ConditionalEffects parse(std::string effect_string) const;
};

#endif /* EFFECT_PARSER_H */
