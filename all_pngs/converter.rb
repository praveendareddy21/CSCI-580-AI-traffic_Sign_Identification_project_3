#!/usr/bin/env ruby
if ARGV.count < 3
    puts 'usage: converter <input dir> <output dir> <size>'
else
    success = 0
    failure = 0
    Dir.foreach(ARGV[0]) do |image|
        next if image == '.' or image == '..'
        spath = ARGV[0]+'/'+image
        epath = ARGV[1]+'/'+image
        size = ARGV[2]
        if system("convert #{spath} -resize #{size}\^ -gravity center -extent #{size} #{epath}") then
            puts spath + ' -> ' + epath
            success += 1
        else
            failure += 1
        end
    end
    puts 'Done!'
    puts 'Converted ' + success.to_s + ' images. ' + failure.to_s + ' failed.'
end