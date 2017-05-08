// Processed by NMI's Java Code Viewer 4.8.2 1997-2000 B. Lemaire
// Website: http://njcv.htmlplanet.com	E-mail: info@njcv.htmlplanet.com
// Copy registered to Evaluation Copy
// Source File Name:   PSIPredViewer.java


public class PSIPredViewer {

    public static void main(String args[]) throws Exception {
        fsspCanvas2 canvas = new fsspCanvas2(args[0]);
        PSGraphics postscript = new PSGraphics();
        for(int page = 0; canvas.print(postscript, page); page++);
    }

    public PSIPredViewer() {
    }
}
