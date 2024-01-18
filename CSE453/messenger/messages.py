########################################################################
# COMPONENT:
#    MESSAGES
# Author:
#    Br. Helfrich, Kyle Mueller, <your name here if you made a change>
# Summary: 
#    This class stores the notion of a collection of messages
########################################################################

import control as c, message

##################################################
# MESSAGES
# The collection of high-tech messages
##################################################
class Messages:

    ##################################################
    # MESSAGES CONSTRUCTOR
    # Read a file to fill the messages
    ##################################################
    def __init__(self, filename):
        self._messages = []
        self._read_messages(filename)

    ##################################################
    # MESSAGES :: DISPLAY
    # Display the list of messages
    ################################################## 
    def display(self, subject_control : c.Control):
        for m in self._messages:
            if c.security_condition_read(m.control, subject_control):
                m.display_properties()

    ##################################################
    # MESSAGES :: SHOW
    # Show a single message
    ################################################## 
    def show(self, id, subject_control : c.Control):
        for m in self._messages:
            if m.get_id() == id:
                if c.security_condition_read(m.control, subject_control):
                    m.display_text()
                else:
                    print("\tCannot view message due to your security level.")
                return True
        return False

    ##################################################
    # MESSAGES :: UPDATE
    # Update a single message
    ################################################## 
    def update(self, id, text, subject_control: c.Control):
        for m in self._messages:
            if m.get_id() == id:
                if c.security_condition_write(m.control, subject_control):
                    m.update_text(text)
                else:
                    print("\tCannot update message due to your security level.")

    ##################################################
    # MESSAGES :: REMOVE
    # Remove a single message
    ################################################## 
    def remove(self, id, subject_control: c.Control):
        for m in self._messages:
            if m.get_id() == id:
                if c.security_condition_write(m.control, subject_control):
                    m.clear()
                else:
                    print("\tCannot remove document due to your security level.")

    ##################################################
    # MESSAGES :: ADD
    # Add a new message
    ################################################## 
    def add(self, text, author, date, control_level):
        m = message.Message(text, author, date, control_level)
        self._messages.append(m)

    ##################################################
    # MESSAGES :: READ MESSAGES
    # Read messages from a file
    ################################################## 
    def _read_messages(self, filename):
        try:
            with open(filename, "r") as f:
                for line in f:
                    text_control, author, date, text = line.split('|')
                    self.add(text.rstrip('\r\n'), author, date, text_control)

        except FileNotFoundError:
            print(f"ERROR! Unable to open file \"{filename}\"")
            return
