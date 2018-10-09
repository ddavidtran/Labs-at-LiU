package server;
// An example class that uses the secure server socket class

import client.MESSAGE;

import java.io.*;
import java.net.*;
import javax.net.ssl.*;
import java.nio.file.Paths;
import java.security.*;
import java.util.StringTokenizer;


public class SecureAdditionServer {
	private int port;
	// This is not a reserved port number
	static final int DEFAULT_PORT = 8191;
	static final String KEYSTORE = "LIUkeystore.ks";
	static final String TRUSTSTORE = "LIUtruststore.ks";
	static final String KEYSTOREPASS = "123456";
	static final String TRUSTSTOREPASS = "abcdef";
    private String currentDir = Paths.get(".").toAbsolutePath().normalize().toString();

    /** Constructor
	 * @param port The port where the server
	 *    will listen for requests
	 */
	SecureAdditionServer( int port ) {
		this.port = port;
	}

	/** The method that does the work for the class */
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

			// SSLServerSocketobject can be created to listen on the specified port
			SSLServerSocketFactory sslServerFactory = sslContext.getServerSocketFactory();
			SSLServerSocket sss = (SSLServerSocket) sslServerFactory.createServerSocket( port );
			sss.setEnabledCipherSuites( sss.getSupportedCipherSuites() );
			
			System.out.println("\n>>>> SecureAdditionServer: active ");
			SSLSocket incoming = (SSLSocket)sss.accept();

			BufferedReader in = new BufferedReader( new InputStreamReader( incoming.getInputStream() ) );
			PrintWriter out = new PrintWriter( incoming.getOutputStream(), true );			
			
			String option;
			while(!(option = in.readLine()).equals("")){
				switch (option) {
					case "1":
						String fileToDownload;
						if(!(fileToDownload = in.readLine()).equals("")){
							download(fileToDownload, out);
						}
						break;
					case "2":
					    String fileToUpload = in.readLine();
                        System.out.println(fileToUpload);
                        String fileLine;
                        while(!(fileLine = in.readLine()).equals("")){
                            uploadGivenFile(fileLine, fileToUpload);
                        }
                        out.println("Uploading file...");
						break;
					case "3":
						out.println("Delete file...");
						break;
					default:
						out.println("Invalid option received from the client.");
						break;
				}
			}
			/*while ( !(str = in.readLine()).equals("") ) {
				double result = 0;
				StringTokenizer st = new StringTokenizer( str );
				try {
					while( st.hasMoreTokens() ) {
						Double d = new Double( st.nextToken() );
						result += d.doubleValue();
					}
					out.println( "The result is " + result );
				}
				catch( NumberFormatException nfe ) {
					out.println( "Sorry, your list contains an invalid number" );
				}
			}*/
			incoming.close();
		}
		catch( Exception x ) {
			System.out.println( x );
			x.printStackTrace();
		}
	}

	/* Lab3 implementation */
	public void download(String filename, PrintWriter client){
        try(BufferedReader br = new BufferedReader(new FileReader(currentDir+"/"+filename));
            FileOutputStream out = new FileOutputStream(currentDir.split("server")[0]+"client/"+filename)){

            String currentLine = "";
            while((currentLine = br.readLine()) != null){
                byte b[]=currentLine.getBytes();//converting string into byte array
                out.write(b);
                out.write("\n".getBytes());
            }
            client.println("Successfully downloaded " + filename + "!");
        }
		catch ( Exception x ){
            client.println(MESSAGE.TERMINATION.name()); //RST message.
            client.println(x);
			x.printStackTrace();
        }
	}

	public void uploadGivenFile(String line, String filename){
        try(FileOutputStream out = new FileOutputStream(currentDir.split("server")[0]+"client/"+filename)){
            byte b[] = line.getBytes();
            out.write(b);
            out.write("\n".getBytes());
        }
        catch(Exception e ){
            System.out.println( e );
            e.printStackTrace();
        }
	}

	public void delete(){

	}
	
	
	/** The test method for the class
	 * @param args[0] Optional port number in place of
	 *        the default
	 */
	public static void main( String[] args ) {
		int port = DEFAULT_PORT;
		if (args.length > 0 ) {
			port = Integer.parseInt( args[0] );
		}
		SecureAdditionServer addServe = new SecureAdditionServer( port );
		addServe.run();
	}
}

