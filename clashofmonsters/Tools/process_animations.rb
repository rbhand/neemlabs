require 'fileutils'

tools_dir = File.dirname(__FILE__)
spritecutter_bin = tools_dir + '/bin/spritecutter'
tmp_dir = '/tmp/pirates-vs-ninjas-process-animations'

units = {
  'octopus' => [ 0, 'large_0' ],
  'skeleton' => [ 0, 'large_1' ],
  'alligator' => [ 0, 'large_2' ],
  'megaoctopus' => [ 0, 'large_3' ],
  'megaskeleton' => [ 0, 'large_4' ],
  'pirate2' => [ 0, 'normal_0' ], # gun
  'pirate1' => [ 0, 'normal_1' ], # sword
  
  'dragon' => [ 1, 'large_0' ],
  'crane' => [ 1, 'large_1' ],
  'spider' => [ 1, 'large_2' ],
  'megadragon' => [ 1, 'large_3' ],
  'megaspider' => [ 1, 'large_4' ],
  'ninja2' => [ 1, 'normal_0' ], # stars
  'ninja1' => [ 1, 'normal_1' ], # stick
}

def copy_frames(unit_dirname_a, animation, output_dirname, unit, color, avatar_dir_suffix='')
  frames = []
  Dir.foreach(unit_dirname_a + '/' + animation) do |frame_filename|
    if frame_filename =~ /^[a-zA-Z0-9]+.png$/ then
      frames << frame_filename
    end
  end
  
  frames.sort.each_index do |i|
    FileUtils.copy(unit_dirname_a + '/' + animation + '/' + frames[i],
      output_dirname + "/Avatar_#{unit[0]}#{avatar_dir_suffix}/avatar_#{unit[0]}_#{unit[1]}_#{color}_#{animation}_#{i}.png")
  end
end


if ARGV.count == 2 then
  input_dirname = ARGV[0]
  output_dirname = ARGV[1]
  
  FileUtils.remove_dir(output_dirname + '/Avatar_0', true)
  FileUtils.remove_dir(output_dirname + '/Avatar_1', true)
  FileUtils.mkdir_p(output_dirname + '/Avatar_0')
  FileUtils.mkdir_p(output_dirname + '/Avatar_1')
  FileUtils.remove_dir(output_dirname + '/Avatar_0_attack', true)
  FileUtils.remove_dir(output_dirname + '/Avatar_1_attack', true)
  FileUtils.mkdir_p(output_dirname + '/Avatar_0_attack')
  FileUtils.mkdir_p(output_dirname + '/Avatar_1_attack')
  
  ['red', 'green', 'blue'].each do |color|
    color_dirname = input_dirname + '/' + color
    
    if File.directory?(color_dirname) then
      print "Color \"#{color}\"\n"
      
      Dir.foreach(color_dirname) do |unit_dirname|
        if unit_dirname =~ /^[a-zA-Z0-9]+$/ then
          unit = units[unit_dirname]
        
          if unit then
            print "\nProcessing \"#{unit_dirname}\" ... \n"
            
            unit_dirname_a = File.expand_path(input_dirname + '/' + color + '/' + unit_dirname)
            
            print "  copying to temporary folder ...\n"
            
            `rm -Rf '#{tmp_dir}'`
            `mkdir '#{tmp_dir}'`
            `cp -Rf #{unit_dirname_a} '#{tmp_dir}'`
            
            unit_dirname_a = tmp_dir + '/' + unit_dirname
            
            folders = [
              unit_dirname_a + '/idle',
              unit_dirname_a + '/move',
              unit_dirname_a + '/attack',
              unit_dirname_a + '/attack_back'
            ]
            
            folders << unit_dirname_a + '/special' if File.directory?(unit_dirname_a + '/special')
            folders << unit_dirname_a + '/special_back' if File.directory?(unit_dirname_a + '/special_back')
            
            print "  cutting sprites ...\n"
            print `#{spritecutter_bin} #{folders.map{|f| "\"#{f}\"" }.join(' ') }`
            
            print "  copying ...\n"
            copy_frames(unit_dirname_a, 'idle', output_dirname, unit, color)
            copy_frames(unit_dirname_a, 'move', output_dirname, unit, color)
            copy_frames(unit_dirname_a, 'attack', output_dirname, unit, color, '_attack')
            copy_frames(unit_dirname_a, 'attack_back', output_dirname, unit, color, '_attack')

            copy_frames(unit_dirname_a, 'special', output_dirname, unit, color) if folders.include? unit_dirname_a + '/special'
            copy_frames(unit_dirname_a, 'special_back', output_dirname, unit, color) if folders.include? unit_dirname_a + '/special_back'
            
          else
            print "ERROR: undefined unit \"#{unit_dirname}\"\n"
          end
        end
      end
      
    else
      print "ERROR: not found \"#{color_dirname}\"\n"
    end
  end
  
else
  print "Arguments expected: input_dir output_dir\n"
end
