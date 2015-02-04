/* -*- mode: java; c-basic-offset: 4; indent-tabs-mode: nil -*- */

package com.google.code.SequantoAutomation;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.regex.*;
import java.io.*;

import processing.app.*;
import processing.core.*;
import processing.app.tools.Tool;

public class SequantoAutomationTool implements Tool
{
    private Editor m_editor;
    private String m_generatorPy;

    public void init(Editor _editor)
    {
        this.m_editor = _editor;

        File toolRoot = new File ( SequantoAutomationTool.class.getProtectionDomain().getCodeSource().getLocation().getPath() ).getParentFile().getParentFile();
        m_generatorPy = new File ( new File ( toolRoot, "generator"), "generate_automation_defines.py" ).getAbsolutePath();
    }

    public String getMenuTitle()
    {
        return "Generate Automation";
    }

    public void run()
    {
        try
        {
            if ( !new File(m_generatorPy).exists() )
            {
                Base.showMessage ( "ERROR", String.format("Could not find generator python script at %s", m_generatorPy) );
            }
            Sketch sketch = m_editor.getSketch();
            //if ( sketch.isModified() )
            //{
            //    sketch.save();
            //}
            //String sketchName = sketch.getName();
            //SketchCode codeObject = sketch.getCurrentCode();
            //String code = codeObject.getProgram();
            String code = m_editor.getText();
            int start = code.indexOf ( "BEGIN AUTOMATION" );
            if ( start != -1 )
            {
                int end = code.indexOf ( "END AUTOMATION", start );
                if ( end != -1 )
                {
                    String automationCode = code.substring ( start + "BEGIN AUTOMATION".length(), end );
                    automationCode = automationCode.replaceAll ( "\\*\\s+", "" );

                    File automationFileName = new File(sketch.prepareCodeFolder(), "automation.automation" );
                    FileWriter writer = new FileWriter ( automationFileName );
                    writer.write ( "name automation\n" );
                    writer.write ( "import " + sketch.getMainFilePath().toString() + "\n" );
                    writer.write ( automationCode );
                    writer.close();
                    try
                    {
                        ProcessBuilder process = new ProcessBuilder(m_generatorPy,automationFileName.getAbsolutePath() );
                        process.directory ( sketch.prepareCodeFolder() );
                        process.start().waitFor();

                        String includeLibLine = "#include \"SequantoAutomation.h\"\n";
                        if ( !code.contains(includeLibLine) )
                        {
                            code = includeLibLine + code;
                        }

                        File generatedFileName = new File(sketch.prepareCodeFolder(), "automation_automation.c" );
                        String includeLine = String.format("#include \"%s\"\n", generatedFileName.getAbsolutePath());
                        if ( !code.contains(includeLine) )
                        {
                            int i = code.indexOf(includeLibLine);
                            code = code.substring(0, i + includeLibLine.length()) +
                                includeLine +
                                code.substring ( i + includeLibLine.length(), code.length() );
                        }
                        if ( m_editor.getText() != code )
                        {
                            System.out.println ( "Setting code to" );
                            System.out.println ( code );
                            System.out.println ( "Current text was" );
                            System.out.println ( m_editor.getText() );
                            m_editor.setText ( code);
                        }
                    }
                    catch ( Exception ex )
                    {
                        Base.showMessage("ERROR", String.format("Could not start generator script: %s", ex) );
                    }
                }
                else
                {
                    Base.showMessage("ERROR", "Can not find END AUTOMATION." );
                }
            }
            else
            {
                Base.showMessage("ERROR", "Can not find BEGIN AUTOMATION." );
            }
        }
        catch ( java.io.IOException ex )
        {
            Base.showMessage("ERROR", "Could not save sketch before trying to generate." );
        }
    }
}
