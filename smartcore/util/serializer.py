

def serialize(dictionary: dict):
    serialized = '['
    serialized += ', '.join(['%s'] * len(dictionary)) % tuple(dictionary.values())
    return serialized + ']'

