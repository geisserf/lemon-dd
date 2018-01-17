#include "effect_parser.h"
#include "conditional_effects.h"
#include "polynomial.h"
#include "utils/string_utils.h"

using std::string;
using std::vector;

std::pair<string, int> EffectParser::get_effect_pair(string &effect) const {
    // !var-> 0, var->1, var==x ->x
    StringUtils::trim(effect);
    if (effect[0] == '!') {
        string var = effect.substr(1);
        int val = 0;
        return {var, val};
    }
    auto pos = effect.find("==");
    if (pos == string::npos) {
        // No '==' in effect, variable is set to 1
        return {effect, 1};
    }
    string var = effect.substr(0, pos);
    StringUtils::trim(var);
    int val = std::stoi(effect.substr(pos + 2));
    return {var, val};
}

ConditionalEffect EffectParser::parse_single_effect(
    string const &effect_string) const {
    auto split_index = effect_string.find("->");
    string conditions = effect_string.substr(0, split_index);
    string effect = effect_string.substr(split_index + 2);
    StringUtils::trim(conditions);
    StringUtils::trim(effect);
    Polynomial formula = Polynomial(conditions);
    std::pair<string, int> effect_pair = get_effect_pair(effect);
    ConditionalEffect result =
        ConditionalEffect(formula, effect_pair.first, effect_pair.second);
    return result;
}

vector<ConditionalEffect> EffectParser::parse(
    string const &effect_string) const {
    vector<ConditionalEffect> result;
    string::size_type pos = 0;
    while (true) {
        string::size_type next_pos = effect_string.find(" & ", pos);
        string effect = effect_string.substr(pos, next_pos - pos);
        // Remove outer parantheses
        result.push_back(
            parse_single_effect(effect.substr(1, effect.size() - 2)));
        if (next_pos == string::npos) {
            break;
        }
        pos = next_pos + 3;
    }
    return result;
}
