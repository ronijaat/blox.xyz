import re
import decimal

def parse_json_with_arbitrary_precision(json_string):
    # Helper function to convert numeric strings to Decimal with arbitrary precision
    def parse_number(num_str):
        if '.' in num_str or 'e' in num_str.lower():
            return decimal.Decimal(num_str)
        else:
            return decimal.Decimal(int(num_str))

    # Helper function to parse JSON values
    def parse_value(value_str):
        if value_str == 'true':
            return True
        elif value_str == 'false':
            return False
        elif value_str == 'null':
            return None
        elif value_str[0] == '"' and value_str[-1] == '"':
            return value_str[1:-1]
        elif value_str[0] == '{' and value_str[-1] == '}':
            return parse_object(value_str[1:-1])
        elif value_str[0] == '[' and value_str[-1] == ']':
            return parse_list(value_str[1:-1])
        else:
            return parse_number(value_str)

    # Helper function to parse JSON objects
    def parse_object(obj_str):
        obj = {}
        key_value_pairs = re.findall(r'("(?:\\.|[^"])*")\s*:\s*(.+?)(?=\s*,\s*|$)', obj_str)
        for key, value_str in key_value_pairs:
            key = key[1:-1]  # Remove quotes from key
            obj[key] = parse_value(value_str.strip())
        return obj

    # Helper function to parse JSON lists
    def parse_list(list_str):
        list_items = re.findall(r'(.+?)(?=\s*,\s*|$)', list_str)
        return [parse_value(item.strip()) for item in list_items]

    return parse_value(json_string.strip())

# Example
json_string = '{"integer": 123456789012345678901234567890, "float": 3.141592653589793238462643383279502884197169399375105820974944}'
parsed_json = parse_json_with_arbitrary_precision(json_string)
print(parsed_json)
print(type(parsed_json))

