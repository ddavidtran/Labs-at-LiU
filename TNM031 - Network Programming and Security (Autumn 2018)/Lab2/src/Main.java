import java.math.BigInteger;

public class Main {

    public static void main(String[] args) {
        Key k1 = new Key("David");
        Key k2 = new Key("Jakke");
        BigInteger Bob_n = k1.getPublicKey(); //Public n of k1
        BigInteger Bob_e = k1.getEncryptExp(); //Public encryption exponent of k2

        String message = "Ursäkta? Det är bokat här.";
        BigInteger alice_c = k2.encryptMessage(message, Bob_e, Bob_n);

        System.out.println("\n");
        String msg = k1.decryptMessage(alice_c);

        System.out.println("\n");
        if(msg.equals(message)){
            System.out.println("RIP IN GG WP");
        }
        else
            System.out.println("Okay, it did not work...rip");
    }
}
