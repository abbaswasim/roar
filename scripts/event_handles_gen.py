EventType = [
        "mouse",
        "keyboard",
        "window"
]

EventCode = [
        "none",
        "n0", "n1", "n2", "n3", "n4", "n5", "n6", "n7", "n8", "n9",
        "num0", "num1", "num2", "num3", "num4", "num5", "num6", "num7", "num8", "num9",
        "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
        "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19",
        "tab", "space", "back_space", "caps_lock", "page", "enter", "del", "arrow", "home", "end", "insert", "esc", "fn"
]

EventModifier = [
        "none",
        "left_mouse",
        "middle_mouse",
        "right_mouse",
        "left_shift",
        "left_control",
        "left_command",
        "left_option",
        "right_shift",
        "right_control",
        "right_command",
        "right_option"
]

EventState = [
        "click",
        "up",
        "down",
        "move",
        "drag",
        "resize",
        "focus"
]

total_vars = 0

for ET in EventType:
    for EC in EventCode:
        for EM in EventModifier:
            for ES in EventState:
                if (ET == "mouse" and EC != "none"):
                    continue
                if (ET == "mouse" and (ES in ["resize", "focus"])):
                    continue
                if (ET == "window" and EC != "none"):
                    continue
                if (ET == "window" and EM != "none"):
                    continue
                total_vars += 1
                print("static const auto " + ET + "_", end = '')
                if (EC != "none"):
                    print(EC +  "_", end = '')
                if (EM != "none"):
                    print(EM +  "_", end = '')
                print(ES, end = ' ')
                print("= create_event_handle(EventType::" + ET + ", EventCode::" + EC + ", EventModifier::" + EM + ", EventState::" + ES + ");")

print( "Total vars = " + str(total_vars))
