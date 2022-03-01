__iota_counter : int = 0

#Using enums like in Golang
def iota(reset : bool = False) -> int:
    global __iota_counter
    if reset:
        __iota_counter = 0
    result = __iota_counter
    __iota_counter += 1
    return result