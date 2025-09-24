import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class FileDecoder {
    
    public static void main(String[] args) {
        if (args.length != 1) {
            System.out.println("Usage: java FileDecoder <encoded_file>");
            System.out.println("Example: java FileDecoder document.pdf.encoded");
            System.exit(1);
        }
        
        String encodedFile = args[0];
        
        // Check if encoded file exists
        Path encodedPath = Paths.get(encodedFile);
        if (!Files.exists(encodedPath)) {
            System.err.println("Error: Encoded file '" + encodedFile + "' not found!");
            System.exit(1);
        }
        
        // Check if encoder_decoder executable exists
        Path decoderPath = Paths.get("./encoder_decoder");
        if (!Files.exists(decoderPath)) {
            System.err.println("Error: encoder_decoder executable not found in current directory!");
            System.err.println("Make sure you're running this from the same directory as encoder_decoder");
            System.exit(1);
        }
        
        System.out.println("=== Java File Decoder ===");
        System.out.println("Invoking C++ decoder for: " + encodedFile);
        System.out.println();
        
        try {
            // Build the command
            ProcessBuilder processBuilder = new ProcessBuilder("./encoder_decoder", "decode", encodedFile);
            processBuilder.redirectErrorStream(true); // Merge stdout and stderr
            
            // Start the process
            Process process = processBuilder.start();
            
            // Read the output
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            
            System.out.println("C++ Decoder Output:");
            System.out.println("-------------------");
            while ((line = reader.readLine()) != null) {
                System.out.println(line);
            }
            
            // Wait for the process to complete
            int exitCode = process.waitFor();
            
            System.out.println("-------------------");
            if (exitCode == 0) {
                // Determine original filename
                String originalFile = encodedFile;
                if (originalFile.endsWith(".encoded")) {
                    originalFile = originalFile.substring(0, originalFile.length() - 8);
                }
                
                // Check if original file was created
                Path originalPath = Paths.get(originalFile);
                if (Files.exists(originalPath)) {
                    long fileSize = Files.size(originalPath);
                    System.out.println("✓ SUCCESS: File decoded successfully!");
                    System.out.println("✓ Original file restored: " + originalFile);
                    System.out.println("✓ File size: " + fileSize + " bytes");
                    
                    // Show file type information
                    String fileType = getFileType(originalFile);
                    System.out.println("✓ File type: " + fileType);
                } else {
                    System.err.println("✗ ERROR: Original file was not created!");
                    System.exit(1);
                }
            } else {
                System.err.println("✗ ERROR: Decoder failed with exit code: " + exitCode);
                System.exit(1);
            }
            
        } catch (IOException e) {
            System.err.println("✗ ERROR: Failed to execute encoder_decoder: " + e.getMessage());
            System.exit(1);
        } catch (InterruptedException e) {
            System.err.println("✗ ERROR: Process was interrupted: " + e.getMessage());
            System.exit(1);
        }
    }
    
    /**
     * Determines file type based on extension
     */
    private static String getFileType(String filename) {
        String extension = "";
        int lastDot = filename.lastIndexOf('.');
        if (lastDot > 0) {
            extension = filename.substring(lastDot).toLowerCase();
        }
        
        switch (extension) {
            case ".txt":
            case ".md":
            case ".cpp":
            case ".java":
            case ".py":
            case ".js":
            case ".html":
            case ".css":
                return "Text file";
            case ".jpg":
            case ".jpeg":
            case ".png":
            case ".gif":
            case ".bmp":
            case ".tiff":
                return "Image file";
            case ".mp4":
            case ".avi":
            case ".mov":
            case ".mkv":
            case ".webm":
                return "Video file";
            case ".mp3":
            case ".wav":
            case ".flac":
            case ".aac":
            case ".ogg":
                return "Audio file";
            case ".pdf":
                return "PDF document";
            case ".doc":
            case ".docx":
                return "Word document";
            case ".xls":
            case ".xlsx":
                return "Excel spreadsheet";
            case ".zip":
            case ".tar":
            case ".gz":
            case ".7z":
            case ".rar":
                return "Archive file";
            case ".exe":
            case ".app":
            case ".deb":
            case ".rpm":
                return "Executable file";
            case ".dat":
            case ".bin":
                return "Binary data file";
            default:
                if (extension.isEmpty()) {
                    return "Executable/Binary file";
                }
                return "Unknown file type (" + extension + ")";
        }
    }
}