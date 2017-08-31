#include "effect_parser.h"
#include "conditional_effects.h"
#include "polynomial.h"

ConditionalEffects EffectParser::parse(std::string effect_string) const {
    int paren_cout = 0;
    int begin = 0;
    int pos = 0;
    std::vector<ConditionalEffect> effects;
    std::string condition;
    std::string effect;

    for (char &c : effect_string) {
        if (c == '(') {
            if (paren_cout == 0)
                begin = pos;
            paren_cout++;
        }
        if (c == ')') {
            paren_cout--;
            if (paren_cout == 0) {
                // remove initial( and trailing )#
                int b = begin + 3 + condition.size();
                effect = effect_string.substr(b, pos - b);

                Polynomial condition_p = Polynomial(condition);
                std::string effect_var = "";
                int effect_val = 1;
                if (effect[0] == '!') {
                    effect_var = effect[1];
                    effect_val = 0;
                } else {
                    effect_var = effect[0];
                }

                ConditionalEffect eff = ConditionalEffect(
                    condition_p.getExpression(), effect_var, effect_val);

                effects.push_back(eff);
            }
        }
        if (c == '-' && effect_string[pos + 1] == '>') {
            condition = effect_string.substr(begin + 1, pos - begin - 1);
        }

        pos++;
    }

    ConditionalEffects ce = ConditionalEffects(effects);
    return ce;
}
