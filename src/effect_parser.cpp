#include "effect_parser.h"
#include "conditional_effects.h"
#include "polynomial.h"

int EffectParser::get_effect_val(std::string effect_string) const {
    // !var-> 0, var->1, var==x ->x

    if (effect_string[0] == '!') {
        return 0;
    } else {
        int pos = 0;
        for (char &c : effect_string) {
            if (c == '=') {
                return std::stoi(effect_string.substr(
                    pos + 2, effect_string.size() - (pos + 2)));
            }
            pos++;
        }
        return 1; // no == in string effect is var ->1
    }
}

std::string EffectParser::get_effect_var(std::string effect_string) const {
    // !var-> var, var->var, var==x ->var

    if (effect_string[0] == '!') {
        return effect_string.substr(1, effect_string.size() - 1);
    } else {
        int pos = 0;
        for (char &c : effect_string) {
            if (c == '=') {
                return effect_string.substr(0, pos);
            }
            pos++;
        }
        return effect_string;
    }
}

std::vector<ConditionalEffect> EffectParser::parse(
    std::string effect_string) const {
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
                Polynomial formula = Polynomial(condition);
                std::string effect_var = get_effect_var(effect);
                int effect_val = get_effect_val(effect);
                ConditionalEffect eff =
                    ConditionalEffect(formula, effect_var, effect_val);
                effects.push_back(eff);
            }
        }
        if (c == '-' && effect_string[pos + 1] == '>') {
            condition = effect_string.substr(begin + 1, pos - begin - 1);
        }

        pos++;
    }

    return effects;
}
