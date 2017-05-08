// Processed by NMI's Java Code Viewer 4.8.2 1997-2000 B. Lemaire
// Website: http://njcv.htmlplanet.com	E-mail: info@njcv.htmlplanet.com
// Copy registered to Evaluation Copy
// Source File Name:   fsspProgFile.java

import java.io.IOException;
import java.util.Vector;

public class fsspProgFile {

    public static fsspData readSecStruct(String fsspFilename) throws IOException {
        fsspData data = psipredReader.readFile(fsspFilename);
        int l = data.getSecondaryStructure().length();
        int c = 0;
        int h = 0;
        int b = 0;
        int t = 0;
        int n = 0;
        Vector CysaposstartVect = new Vector();
        Vector CysaposfinishVect = new Vector();
        Vector HposstartVect = new Vector();
        Vector HposfinishVect = new Vector();
        Vector BposstartVect = new Vector();
        Vector BposfinishVect = new Vector();
        Vector TposstartVect = new Vector();
        Vector TposfinishVect = new Vector();
        Vector NposstartVect = new Vector();
        Vector NposfinishVect = new Vector();
        if(data.getSecondaryStructure().charAt(0) == 'H')
            HposstartVect.addElement(new Integer(0));
        if(data.getSecondaryStructure().charAt(0) == 'G')
            HposstartVect.addElement(new Integer(0));
        if(data.getSecondaryStructure().charAt(0) == 'E')
            BposstartVect.addElement(new Integer(0));
        if(data.getSecondaryStructure().charAt(0) == 'T')
            TposstartVect.addElement(new Integer(0));
        if(data.getSecondaryStructure().charAt(0) == 'S')
            TposstartVect.addElement(new Integer(0));
        if(data.getSecondaryStructure().charAt(0) == ' ')
            NposstartVect.addElement(new Integer(0));
        if(data.getSecondaryStructure().charAt(0) == 'B')
            NposstartVect.addElement(new Integer(0));
        if(data.getSecondaryStructure().charAt(0) == 'C')
            NposstartVect.addElement(new Integer(0));
        do {
            data.getSecondaryStructure().charAt(c);
            if(c != 0) {
                Integer a1 = new Integer(c);
                Integer b1 = new Integer(c);
                Integer t1 = new Integer(c);
                Integer n1 = new Integer(c);
                if(data.getSecondaryStructure().charAt(c - 1) != 'H' && data.getSecondaryStructure().charAt(c) == 'H')
                    HposstartVect.addElement(a1);
                if(data.getSecondaryStructure().charAt(c - 1) != 'G' && data.getSecondaryStructure().charAt(c) == 'G')
                    HposstartVect.addElement(a1);
                if(data.getSecondaryStructure().charAt(c - 1) != 'I' && data.getSecondaryStructure().charAt(c) == 'I')
                    HposstartVect.addElement(a1);
                if(data.getSecondaryStructure().charAt(c - 1) != 'E' && data.getSecondaryStructure().charAt(c) == 'E')
                    BposstartVect.addElement(b1);
                if(data.getSecondaryStructure().charAt(c - 1) != 'T' && data.getSecondaryStructure().charAt(c) == 'T')
                    TposstartVect.addElement(t1);
                if(data.getSecondaryStructure().charAt(c - 1) != 'S' && data.getSecondaryStructure().charAt(c) == 'S')
                    TposstartVect.addElement(t1);
                if(data.getSecondaryStructure().charAt(c - 1) != ' ' && data.getSecondaryStructure().charAt(c) == ' ')
                    NposstartVect.addElement(n1);
                if(data.getSecondaryStructure().charAt(c - 1) != 'B' && data.getSecondaryStructure().charAt(c) == 'B')
                    NposstartVect.addElement(n1);
                if(data.getSecondaryStructure().charAt(c - 1) != 'C' && data.getSecondaryStructure().charAt(c) == 'C')
                    NposstartVect.addElement(n1);
            }
            if(c + 1 < l) {
                Integer a2 = new Integer(c);
                Integer b2 = new Integer(c);
                Integer t2 = new Integer(c);
                Integer n2 = new Integer(c);
                if(data.getSecondaryStructure().charAt(c + 1) != 'H' && data.getSecondaryStructure().charAt(c) == 'H')
                    HposfinishVect.addElement(a2);
                if(data.getSecondaryStructure().charAt(c + 1) != 'H' && data.getSecondaryStructure().charAt(c) == 'H')
                    h++;
                if(data.getSecondaryStructure().charAt(c + 1) != 'G' && data.getSecondaryStructure().charAt(c) == 'G')
                    HposfinishVect.addElement(a2);
                if(data.getSecondaryStructure().charAt(c + 1) != 'G' && data.getSecondaryStructure().charAt(c) == 'G')
                    h++;
                if(data.getSecondaryStructure().charAt(c + 1) != 'I' && data.getSecondaryStructure().charAt(c) == 'I')
                    HposfinishVect.addElement(a2);
                if(data.getSecondaryStructure().charAt(c + 1) != 'I' && data.getSecondaryStructure().charAt(c) == 'I')
                    h++;
                if(data.getSecondaryStructure().charAt(c + 1) != 'E' && data.getSecondaryStructure().charAt(c) == 'E')
                    BposfinishVect.addElement(b2);
                if(data.getSecondaryStructure().charAt(c + 1) != 'T' && data.getSecondaryStructure().charAt(c) == 'T')
                    TposfinishVect.addElement(t2);
                if(data.getSecondaryStructure().charAt(c + 1) != 'T' && data.getSecondaryStructure().charAt(c) == 'T')
                    t++;
                if(data.getSecondaryStructure().charAt(c + 1) != 'S' && data.getSecondaryStructure().charAt(c) == 'S')
                    TposfinishVect.addElement(t2);
                if(data.getSecondaryStructure().charAt(c + 1) != 'S' && data.getSecondaryStructure().charAt(c) == 'S')
                    t++;
                if(data.getSecondaryStructure().charAt(c + 1) != ' ' && data.getSecondaryStructure().charAt(c) == ' ')
                    NposfinishVect.addElement(n2);
                if(data.getSecondaryStructure().charAt(c + 1) != 'B' && data.getSecondaryStructure().charAt(c) == 'B')
                    NposfinishVect.addElement(n2);
                if(data.getSecondaryStructure().charAt(c + 1) != 'B' && data.getSecondaryStructure().charAt(c) == 'B')
                    n++;
                if(data.getSecondaryStructure().charAt(c + 1) != 'C' && data.getSecondaryStructure().charAt(c) == 'C')
                    NposfinishVect.addElement(n2);
            }
            if(data.getSecondaryStructure().charAt(c) == 'H')
                h++;
            if(data.getSecondaryStructure().charAt(c) == 'G')
                h++;
            if(data.getSecondaryStructure().charAt(c) == 'E')
                b++;
            if(data.getSecondaryStructure().charAt(c) == 'T')
                t++;
            if(data.getSecondaryStructure().charAt(c) == 'S')
                t++;
        } while(++c < l);
        if(data.getSecondaryStructure().charAt(l - 1) == 'H')
            HposfinishVect.addElement(new Integer(l - 1));
        if(data.getSecondaryStructure().charAt(l - 1) == 'G')
            HposfinishVect.addElement(new Integer(l - 1));
        if(data.getSecondaryStructure().charAt(l - 1) == 'E')
            BposfinishVect.addElement(new Integer(l - 1));
        if(data.getSecondaryStructure().charAt(l - 1) == 'T')
            TposfinishVect.addElement(new Integer(l - 1));
        if(data.getSecondaryStructure().charAt(l - 1) == 'S')
            TposfinishVect.addElement(new Integer(l - 1));
        if(data.getSecondaryStructure().charAt(l - 1) == ' ')
            NposfinishVect.addElement(new Integer(l - 1));
        if(data.getSecondaryStructure().charAt(l - 1) == 'B')
            NposfinishVect.addElement(new Integer(l - 1));
        if(data.getSecondaryStructure().charAt(l - 1) == 'C')
            NposfinishVect.addElement(new Integer(l - 1));
        data.setVectors(HposstartVect, HposfinishVect, BposstartVect, BposfinishVect, TposstartVect, TposfinishVect, NposstartVect, NposfinishVect, CysaposstartVect, CysaposfinishVect, h, b, t, l);
        return data;
    }

    public fsspProgFile() {
    }
}
