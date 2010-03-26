import org.antlr.runtime.*;
import org.antlr.runtime.tree.*;
import org.antlr.stringtemplate.*;
import java.io.*;

public class FFITool {
    public void cli(String progname, String args[]) throws Exception {
        InputStreamReader stgfile = null;
        String filename = null;
        // check arguments
        if (args.length == 2) {
            stgfile = new FileReader(args[0]);
            filename = args[1];
        } else if (args.length == 1) {
            InputStream stgres = this.getClass().getResourceAsStream("ffi.stg");
            if (stgres == null) {
                throw new FileNotFoundException("ffi.stg (No such resource file.)");
            }
            stgfile = new InputStreamReader(stgres);
            filename = args[0];
        } else {
            System.err.println("Usage:");
            System.err.println("   java " + progname
                               + " [path/to/template.stg] path/to/input");
            System.exit(0);
        }
        StringTemplateGroup stg = new StringTemplateGroup(stgfile);
        parse_file(filename, stg);
    }

    public void parse_file(String filename, StringTemplateGroup stg) throws Exception {
        // create AST
        System.err.println("creating abstract syntax tree...");
        ANTLRFileStream instream = new ANTLRFileStream(filename);
        FFILexer lexer = new FFILexer(instream);
        CommonTokenStream tokens = new CommonTokenStream(lexer);
        FFIParser parser = new FFIParser(tokens);
        Tree tree = (Tree)parser.ffi().tree;
        if (parser.getNumberOfSyntaxErrors() > 0) {
            System.exit(-1);
        }
        // parse the AST
        parse_tree(tree, tokens, stg);
    }

    public void parse_tree(Tree tree, CommonTokenStream tokens, StringTemplateGroup stg) throws Exception {
        // for debugging:
        //System.err.println(t.toStringTree());
        // optimize AST (multiple passes if needed)
        FFITreeOpt opt;
        do {
            System.err.println("optimization pass...");
            CommonTreeNodeStream nodes = new CommonTreeNodeStream(tree);
            nodes.setTokenStream(tokens);
            opt = new FFITreeOpt(nodes);
            tree = (Tree)opt.ffi().tree;
            if (opt.getNumberOfSyntaxErrors() > 0) {
                System.exit(-1);
            }
        } while (opt.again);

        // generate ffi code
        System.err.println("generating ffi file...");
        CommonTreeNodeStream nodes = new CommonTreeNodeStream(tree);
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
