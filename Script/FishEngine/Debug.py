import traceback

class Debug:
    @staticmethod
    def Log(*objs):
        print(*objs)
        for line in traceback.format_stack(limit=2):
            print(line.strip())