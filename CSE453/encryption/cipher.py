##############################################################################
# COMPONENT:
#    CIPHER01
# Author:
#    Br. Helfrich, Kyle Mueller, Journey Curtis
# Summary:
#    Implement your cipher here. You can view 'example.py' to see the
#    completed Caesar Cipher example.
##############################################################################


##############################################################################
# CIPHER
##############################################################################
class Cipher:
    def __init__(self):
        # TODO: Insert anything you need for your cipher here
        pass

    def get_author(self):
        return "Journey Curtis"

    def get_cipher_name(self):
        return "Viginere cipher"

    ############################################################
    # GET CIPHER CITATION
    # Returns the citation from which we learned about the cipher
    ############################################################
    def get_cipher_citation(self):
        s = "1. \"Online Help.\" Vigenere Cipher - Maple Help, " \
            "https://www.maplesoft.com/support/help/maple/view.aspx?path=MathApps%2FVigenereCipher.\n"
        
        s += "2. \"Vigenère Cipher.\" Wikipedia, Wikimedia Foundation, " \
            "22 Mar. 2023, https://en.wikipedia.org/wiki/Vigen%C3%A8re_cipher."

        return s

    ############################################################
    # GET PSEUDOCODE
    # Returns the pseudocode as a string to be used by the caller
    ############################################################
    def get_pseudocode(self):

        # The encrypt pseudocode
        pc = "encrypt(plaintext, password)\n" \
             "   ciphertext = \"\"\n" \
             "   password = verify_password_length(plaintext, password)\n" \
             "   FOR i = 0 to plaintext.length by 1\n" \
             "      cipher_char = (ord(plaintext[i]) + ord(password[i])) % 95\n" \
             "      ciphertext += chr(cipher_char + ord(' '))\n" \
             "   RETURN ciphertext\n"

        # The decrypt pseudocode
        pc += "decrypt(ciphertext, password)\n" \
              "   plaintext = \"\"\n" \
              "   password = verify_password_length(ciphertext, password)\n" \
              "   FOR i = 0 to ciphertext.length by 1\n" \
              "      plain_char = (ord(ciphertext[i]) - ord(password[i]) + 31) % 95\n" \
              "      plaintext += chr(plain_char + ord(' '))\n" \
              "   RETURN plaintext\n"
        
        pc += "verify_password_length(string, password)\n" \
              "   IF string.length == pasword.length\n" \
              "      RETURN password\n" \
              "   old = password\n" \
              "   index = 0\n" \
              "   FOR 0 to (string.length - password.length) by 1\n" \
              "      password += old[index]\n" \
              "      index += 1\n" \
              "      IF index >= old.length\n" \
              "         index = 0\n" \
              "   RETURN password\n"

        return pc

    def encrypt(self, plaintext: str, password):
        """Encrypts the plaintext and return it's ciphertext version"""
        ciphertext = ""
        password = self._verify_password_length(plaintext, password)
        
        for i in range(len(plaintext)):
            cipher_char = (ord(plaintext[i]) + ord(password[i])) % 95
            ciphertext += chr(cipher_char + ord(' '))

        return ciphertext

    def decrypt(self, ciphertext, password):
        """Decrypts the ciphertext and returns the converted plaintext"""
        plaintext = ""
        password = self._verify_password_length(ciphertext, password)

        for i in range(len(ciphertext)):
            # switch everything to numbers and perform the math
            # add 31 due to ' ' being the first char
            plain_char = (ord(ciphertext[i]) - ord(password[i]) + 31) % 95
            plaintext += chr(plain_char + ord(' '))

        return plaintext
    
    def _verify_password_length(self, string, password):
        """
        Verifies that the password length is equal to the string.\n
        If it isn't, then the password is repeated until it matches\n
        lengths.
        """
        if len(string) == len(password):
            return password
        
        old = password
        index = 0
        for _ in range(len(string) - len(password)):
            password += old[index]
            index += 1
            if index >= len(old):
                index = 0

        return password