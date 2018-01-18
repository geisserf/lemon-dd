#ifndef EFFECT_PARSER_H
#define EFFECT_PARSER_H

#include <string>
#include <vector>

class ConditionalEffect;

class EffectParser {
public:
    ConditionalEffect parse_single_effect(
        std::string const &effect_string) const;

    std::vector<ConditionalEffect> parse(
        std::string const &effect_string) const;

private:
    // Reads variable/effect pair from effect string
    std::pair<std::string, int> get_effect_pair(std::string &effect) const;
};

#endif /* EFFECT_PARSER_H */
