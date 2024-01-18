import unittest

class NamesTestCase(unittest.TestCase):

   def test_gen_query(self):
       username= "W3ST0n"
       password= "G_v!n"
       Output="SELECT " + username + " FROM users WHERE passwords == " + password + ";"

def gen_query(username: str, password: str):
    return f"SELECT {username} FROM users where password == {password}"