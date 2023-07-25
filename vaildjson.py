import json
import decimal

def parse_json_with_arbitrary_precision(json_string):
    return json.loads(json_string, parse_float=decimal.Decimal)

# Example usage:
json_string = '{"pi": 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679}'
parsed_data = parse_json_with_arbitrary_precision(json_string)

print(parsed_data)
print(type(parsed_data["pi"]))  # Check the data type of the parsed pi value
