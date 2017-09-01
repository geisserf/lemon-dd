#ifndef EFFECT_PARSER_H
#define EFFECT_PARSER_H
#include <string>

class ConditionalEffect;
class ConditionalEffects;

class EffectParser {
public:
    ConditionalEffects parse(std::string effect_string) const;

private:
    int get_effect_val(std::string effect_string) const;
    std::string get_effect_var(std::string effect_string) const;
};

#endif /* EFFECT_PARSER_H */
