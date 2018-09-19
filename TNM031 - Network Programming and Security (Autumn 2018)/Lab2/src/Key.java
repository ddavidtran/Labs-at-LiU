import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;


public class Key {
    //Static global variable for possible encryption exponents.
    private static ArrayList<Integer> exponents = new ArrayList<Integer>(Arrays.asList(3,5,17,257,65537));
    //Private variables
    private BigInteger p_, q_, e_, d_, omega_, n_;
    private String name_;

    public Key(String name){
        //Constructor of key class. Generates public key, encrypt/decrypt exponents
        name_ = name;
        generatePublicKey();
        generateEncryptExp();
        generateDecryptExp();
    }


    private void generatePublicKey(){
        int bitLength = 512;
        Random rnd = new Random();
        p_ = BigInteger.probablePrime(bitLength, rnd);
        q_ = BigInteger.probablePrime(bitLength, rnd);
        n_ = p_.multiply(q_); //Bit length = 1024
    }


    private void generateEncryptExp(){
        if(p_ == null || q_ == null){
            System.out.println("Please generate public key first!!");
            System.exit(0);
        }
        BigInteger p1 = p_.subtract(BigInteger.ONE); //(p-1)
        BigInteger q1 = q_.subtract(BigInteger.ONE); //(q-1)
        omega_ = p1.multiply(q1); //omega = (p-1)*(q-1)
        for(int exp : exponents){
            e_ = BigInteger.valueOf(exp);
            if(e_.gcd(omega_).equals(BigInteger.ONE)) { // If gcd(e, omega) == 1 then choose that e.
                break;
            }
        }
    }


    private void generateDecryptExp(){
        if(p_ == null || q_ == null || e_ == null){
            System.out.println("Please generate public key first!!");
            System.exit(0);
        }
        else if (e_ == null || omega_ == null){
            System.out.println("Please generate encryption exponent first!!");
            System.exit(0);
        }
        d_ = e_.modInverse(omega_); // (1/e)*mod(omega)
    }


    public BigInteger getPublicKey(){
        if(n_== null){
            System.out.println("Please generate public key first!!");
            return null;
        }
        return n_;
    }


    public BigInteger getEncryptExp(){
        if(e_ == null){
            System.out.println("Please generate public key first!!");
            return null;
        }
        return e_;
    }


    public BigInteger encryptMessage(String message, BigInteger in_e, BigInteger in_n){
        System.out.println(name_ + " encrypts the following message: " + message);
        BigInteger m = new BigInteger(message.getBytes());
        System.out.println(name_ + " converts message to bytes: " + m);
        BigInteger c = m.pow(in_e.intValue()).mod(in_n);
        System.out.println(name_ + " encrypted message: " + c);
        return c;
    }


    public String decryptMessage(BigInteger c){
        System.out.println(name_ + " receives encrypted message: " + c);
        BigInteger decryptMsg = c.modPow(d_,n_);
        System.out.println(name_ + " decrypts message in bytes: " + decryptMsg);
        String message = new String(decryptMsg.toByteArray());
        System.out.println(name_ + " converts bytes to message: " + message);
        return message;
    }



}
