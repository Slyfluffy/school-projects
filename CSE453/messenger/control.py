########################################################################
# COMPONENT:
#    CONTROL
# Author:
#    Br. Helfrich, Kyle Mueller, Journey Curtis
# Summary:
#    This class stores the notion of Bell-LaPadula
########################################################################

# you may need to put something here...
from enum import Enum

#########################################################################
# Assign pre-defined control levels.  by convention.
# The higher the number the more aaccess is allowed.
# Using a enumeration
#########################################################################
class Control(Enum):
    Public = 0
    Confidential = 1
    Privileged = 2
    Secret = 3

#####################################################################
# Returns the control level of a user based on username and password.
# If the username/password combo does not match a recorded user
# a public control level is returned
#####################################################################
def authenticate(username: str, password: str, users) -> Control:
    """Returns the control level of a user based on username and password.
    \n\tIf the username/password combo does not match a recorded user
    \n\ta public control level is returned."""
    for user in users:
        if username == user.name and password == user.password:
            return user.control
    return Control.Public


#####################################################################
# Determines if the subject has the correct privileges to read
#####################################################################
def security_condition_read(asset_control: Control, subject_control: Control) -> bool:
    """Determines if the subject has the correct privileges to read"""
    return subject_control.value >= asset_control.value


###########################################################################
# Determines if the subject has the correct privileges to write
###########################################################################
def security_condition_write(asset_control: Control, subject_control: Control) -> bool:
    """Determines if the subject has the correct privileges to write"""
    return subject_control.value <= asset_control.value

# Read Tests - Display
# 1. Higher to lower
# 2. Same level
# 3. Lower to higher

# Read Tests - Show
# 1. Higher to lower
# 2. Same level
# 3. Lower to higher

# Write Tests - Update
# 1. High to low
# 2. Same Level
# 3. low to high

# Write tests - remove
# 1. high to low
# 2. same level
# 3. low to high

# write test - add
# 1. Adding a message applies user's security level