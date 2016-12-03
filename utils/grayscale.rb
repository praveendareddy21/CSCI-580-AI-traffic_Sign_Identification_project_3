#!/usr/bin/env ruby
if ARGV.count < 1
    puts 'usage: converter <input/output dir>'
else
    success = 0
    failure = 0
    Dir.foreach(ARGV[0]) do |image|
        next if image == '.' or image == '..'
        spath = ARGV[0]+'/'+image
        epath = ARGV[0]+'-bw/'+image
        if system("convert #{spath} -set colorspace Gray -separate -average #{epath}") then
            puts spath + ' -> ' + epath
            success += 1
        else
            failure += 1
        end
    end
    puts 'Done!'
    puts 'Converted ' + success.to_s + ' images. ' + failure.to_s + ' failed.'
end