import org.antlr.runtime.*;
import org.antlr.runtime.tree.*;
import org.antlr.stringtemplate.*;
import java.io.*;

public class xml2ffi {
    public static void main(String args[]) throws Exception {
        // check number of arguments
        if (args.length != 2) {
            System.err.println("       java xml2ffi path/to/template.stg path/to/input");
            System.exit(0);
        }
        FileReader stgfile = new FileReader(args[0]);
        StringTemplateGroup ffistg = new StringTemplateGroup(stgfile);
        stgfile.close();
        translate(new File(args[1]), ffistg);
    }

    public static void translate(File input, StringTemplateGroup stg) throws Exception {
        // create AST
        Tree t = null;
        System.err.println("creating abstract syntax tree...");
        ANTLRFileStream instream = new ANTLRFileStream(input.toString());
        XMLLexer lexer = new XMLLexer(instream);
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        XMLParser parser = new XMLParser(tokens);
        t = (Tree)parser.ffi().tree;
        if (parser.getNumberOfSyntaxErrors() > 0) {
            System.exit(-1);
        }
        // for debugging:
        //System.err.println(r.tree.toStringTree());

        // optimize AST (multiple passes if needed)
        FFITreeOpt opt;
        do {
            System.err.println("optimization pass...");
            CommonTreeNodeStream nodes = new CommonTreeNodeStream(t);
            nodes.setTokenStream(tokens);
            opt = new FFITreeOpt(nodes);
            t = (Tree)opt.ffi().tree;
            if (opt.getNumberOfSyntaxErrors() > 0) {
                System.exit(-1);
            }
        } while (opt.again);

        // generate ffi code
        System.err.println("generating ffi file...");
        CommonTreeNodeStream nodes = new CommonTreeNodeStream(t);
        nodes.setTokenStream(tokens);
        FFITreeTemplate gen = new FFITreeTemplate(nodes);
        gen.setTemplateLib(stg);
        FFITreeTemplate.ffi_return r = gen.ffi();
        if (gen.getNumberOfSyntaxErrors() > 0) {
            System.exit(-1);
        }
        System.out.println(r.st.toString());
    }
}
