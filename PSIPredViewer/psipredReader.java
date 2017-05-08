// Processed by NMI's Java Code Viewer 4.8.2 1997-2000 B. Lemaire
// Website: http://njcv.htmlplanet.com	E-mail: info@njcv.htmlplanet.com
// Copy registered to Evaluation Copy
// Source File Name:   psipredReader.java

import java.io.*;
import java.util.Vector;

public class psipredReader {

    public static fsspData readFile(String fsspFilename) throws IOException {
        BufferedReader in = new BufferedReader(new FileReader(fsspFilename));
        int i = 0;
        StringBuffer buf = new StringBuffer();
        StringBuffer buf2 = new StringBuffer();
        StringBuffer buf3 = new StringBuffer();
        StringBuffer buf4 = new StringBuffer();
        Vector vect5 = new Vector();
        String strNoOfLine;

	      //handle new file format 10/11/03
	      strNoOfLine = in.readLine();
	      strNoOfLine = in.readLine();

        while((strNoOfLine = in.readLine()) != null)  {
            i++;
            char sec = strNoOfLine.charAt(7);
            buf.append(strNoOfLine.charAt(5));
            buf2.append(sec);
            buf3.append(" ");
            buf4.append(" ");
            if(sec == 'C')
                vect5.addElement(new Float(strNoOfLine.substring(11, 16)));
            else
            if(sec == 'H')
                vect5.addElement(new Float(strNoOfLine.substring(18, 23)));
            else
            if(sec == 'E')
                vect5.addElement(new Float(strNoOfLine.substring(25, 30)));
            else
                vect5.addElement(new Float(0.0D));
        }
        fsspData data = new fsspData(buf.toString(), buf2.toString(), buf3.toString(), buf4.toString(), vect5, i);
        return data;
    }

    public psipredReader() {
    }
}
