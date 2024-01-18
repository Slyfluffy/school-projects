using System;
using System.Security.Cryptography;
using System.Text;
using System.Threading;

public class SecurityExample
{
    private static int counter = 0;

    public static void UpdateCounter()
    {
        Console.WriteLine("Updating counter += 1");
        counter++;
        Console.WriteLine("Counter is now " + counter + "\n");
    }

    public static void Reset()
    {
        Console.WriteLine("Resetting counter...");
        Thread.Sleep(5000); // sleep for 1 second to provide example
        counter = 0;
    }

    public static void NormalFunction()
    {
        for (int i = 0; i < 10; i++)
        {
            UpdateCounter();
        }

        Console.WriteLine("Counter before reset: " + counter);
        Reset(); // Reset the data so no bad guys can get it
        Console.WriteLine("Counter after reset: " + counter);
    }

    public static void BadFunction()
    {
        Thread.Sleep(2000); // A thread or time "delay"
        Console.WriteLine("Grabbing counter before reset");
        Console.WriteLine("Counter: " + counter);
        Console.WriteLine("Data obtained!");
    }

    private static byte[] Encrypt(string plainText, byte[] Key, byte[] IV)
    {
        byte[] encrypted;

        using (Aes aes = Aes.Create())
        {
            aes.Key = Key;
            aes.IV = IV;

            ICryptoTransform encryptor = aes.CreateEncryptor(aes.Key, aes.IV);
            using MemoryStream ms = new();
            using CryptoStream cs = new(ms, encryptor, CryptoStreamMode.Write);
            using (StreamWriter sw = new(cs))
            {
                sw.Write(plainText);
            }
            encrypted = ms.ToArray();
        }

        return encrypted;
    }

    private static string Decrypt(byte[] cipherText, byte[] Key, byte[] IV)
    {
        string plainText;

        using (Aes a = Aes.Create())
        {
            a.Key = Key;
            a.IV = IV;

            ICryptoTransform decryptor = a.CreateDecryptor(a.Key, a.IV);

            using MemoryStream ms = new(cipherText);
            using CryptoStream cs = new(ms, decryptor, CryptoStreamMode.Read);
            using StreamReader sr = new(cs);
            plainText = sr.ReadToEnd();
        }

        return plainText;
    }

    public static void Main()
    {
        Console.WriteLine("-- RACE CONDITION EXAMPLE -- \n");
        Console.WriteLine("- Unsafe example -\n");
        Thread t1 = new(new ThreadStart(NormalFunction));
        Thread t2 = new(new ThreadStart(BadFunction));

        t1.Start();
        t2.Start();

        t1.Join();
        t2.Join();

        Console.WriteLine("\n- Safe example -\n");

        Thread t3 = new(new ThreadStart(NormalFunction));
        Thread t4 = new(new ThreadStart(BadFunction));

        t3.Start();
        t3.Join(); // This causes it to synchronize
        t4.Start();
        t4.Join(); 

        Console.WriteLine("-- ENCRYPTION EXAMPLE -- \n");
        
        string plainText = "plainText";
        Console.WriteLine("Before encryption: " + plainText);
        using Aes a = Aes.Create();
        byte[] encrypted = Encrypt(plainText, a.Key, a.IV);
        Console.WriteLine("Encrypted: " + Encoding.Default.GetString(encrypted));
        string decrypted = Decrypt(encrypted, a.Key, a.IV);
        Console.WriteLine("Decrypted: " + decrypted);
    }
}