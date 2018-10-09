package client;// A client-side class that uses a secure TCP/IP socket

import java.io.*;
import java.net.*;
import java.nio.file.Paths;
import java.security.KeyStore;
import java.sql.SQLOutput;
import java.util.Scanner;
import javax.net.ssl.*;


public class SecureAdditionClient {
	private InetAddress host;
	private int port;
	// This is not a reserved port number 
	static final int DEFAULT_PORT = 8191;
	static final String KEYSTORE = "LIUkeystore.ks";
	static final String TRUSTSTORE = "LIUtruststore.ks";
	static final String KEYSTOREPASS = "123456";
	static final String TRUSTSTOREPASS = "abcdef";
    private String currentDir = Paths.get(".").toAbsolutePath().normalize().toString();


    // Constructor @param host Internet address of the host where the server is located
	// @param port Port number on the host where the server is listening
	public SecureAdditionClient( InetAddress host, int port ) {
		this.host = host;
		this.port = port;
	}

  // The method used to start a client object
	public void run() {
		try {

		    //Creating empty keystore object and load KEYSTORE file.
			KeyStore ks = KeyStore.getInstance( "JCEKS" );
			ks.load( new FileInputStream( KEYSTORE ), KEYSTOREPASS.toCharArray() );

            //Creating empty truststore object and load TRUSTSTORE file.
            KeyStore ts = KeyStore.getInstance( "JCEKS" );
			ts.load( new FileInputStream( TRUSTSTORE ), TRUSTSTOREPASS.toCharArray() );

            /*The SSL connection will require access to encryption keys and certificates. For that reason,
            factory objects to create both KeyManager and TrustManager objects are created
            and then initialized with the KeyStore and TrustStore objects: */
            KeyManagerFactory kmf = KeyManagerFactory.getInstance( "SunX509" );
			kmf.init( ks, KEYSTOREPASS.toCharArray() );

			TrustManagerFactory tmf = TrustManagerFactory.getInstance( "SunX509" );
			tmf.init( ts );

			//SSLContext object, using TLS (Transport Layer Security)
			SSLContext sslContext = SSLContext.getInstance( "TLS" );
			sslContext.init( kmf.getKeyManagers(), tmf.getTrustManagers(), null );

            /*SSLSocket object is created to connect
            with the specified host using the port identified*/
			SSLSocketFactory sslFact = sslContext.getSocketFactory();
			SSLSocket client =  (SSLSocket)sslFact.createSocket(host, port);
			client.setEnabledCipherSuites( client.getSupportedCipherSuites() );
			System.out.println("\n>>>> SSL/TLS handshake completed");


			BufferedReader socketIn;
			socketIn = new BufferedReader( new InputStreamReader( client.getInputStream() ) );
			PrintWriter socketOut = new PrintWriter( client.getOutputStream(), true );

			/* Lab3 implementation */
            String option = "";
            Scanner scan = new Scanner(System.in);
			do {
                displayMenu();
				option = scan.nextLine().toLowerCase();
				switch (option) {
					case "1":
						socketOut.println(option);
						System.out.println("Please enter the filename to download: ");
                        String fileToDownload = scan.nextLine();
                        try {
                            socketOut.println(fileToDownload); //Send filename to server via SSL
                            if(socketIn.readLine().equals(MESSAGE.TERMINATION.name())){
                                System.out.println(socketIn.readLine());
                                return;
                            }
                        }catch (Exception e) {
                            System.out.println(e);
                            e.printStackTrace();
                            return;
                        }
                        System.out.println(socketIn.readLine());
                        break;

					case "2":
						socketOut.println(option);
                        System.out.println("Please enter the filename to upload: ");
                        String fileToUpload = scan.nextLine();
                        try{
                            socketOut.println(currentDir+"/"+fileToUpload);  //Send filepath to server via SSL
                            if(socketIn.readLine().equals(MESSAGE.TERMINATION.name())){
                                System.out.println(socketIn.readLine());
                                return;
                            }
                            System.out.println(socketIn.readLine());
                            break;
                        } catch ( Exception e ){
                            System.out.println(e);
                            e.printStackTrace();
                            return;
                        }

					case "3":
						socketOut.println(option);
                        System.out.println("Please enter the filename to delete: ");
                        String fileToDelete = scan.nextLine();
                        try{
                            socketOut.println(fileToDelete);
                            if(MESSAGE.TERMINATION.name().equals(socketIn.readLine())){
                                System.out.println(socketIn.readLine());
                                return;
                            }
                            System.out.println(socketIn.readLine());
                            break;
                        }catch ( Exception e ){
                            System.out.println(e);
                            e.printStackTrace();
                            return;
                        }
				}
			} while (!option.equals("q")); // end of loop

		}
		catch( Exception e ) {
			System.out.println( e );
			e.printStackTrace();
		}
	}

	public void displayMenu(){
		System.out.println("\n*******************************");
		System.out.println("Lab 3: Secure Sockets (SSL/TLS)");
		System.out.println("Author: David Tran (davtr766)");
		System.out.println("*******************************\n");

		System.out.println("1. Download a file from the server.");
		System.out.println("2. Upload a file to the server.");
		System.out.println("3. Delete a file from the server.");
		System.out.println("4. Print menu to console");
		System.out.println("'q' to terminate the program!\n");
		System.out.println("Enter here: ");
	}
	
	// The test method for the class @param args Optional port number and host name
	public static void main( String[] args ) {
		try {
			InetAddress host = InetAddress.getLocalHost();
			int port = DEFAULT_PORT;
			if ( args.length > 0 ) {
				port = Integer.parseInt( args[0] );
			}
			if ( args.length > 1 ) {
				host = InetAddress.getByName( args[1] );
			}
			SecureAdditionClient addClient = new SecureAdditionClient( host, port );
			addClient.run();
		}
		catch ( UnknownHostException uhx ) {
			System.out.println( uhx );
			uhx.printStackTrace();
		}
	}
}
