hex_str = ARGV[0]	

hex_arr = hex_str.reverse().split(//)
result_i = 0
hex_arr.each_index do |h|
  
  power_value =  (0...h).inject(1){|a,n| a*16}
  puts hex_arr[h].hex.to_i
  tmp_value =  (hex_arr[h].hex.to_i) * (power_value.to_i)

  if(h==7)
      #to do ................
       
      if(hex_arr[h].hex >= 8)
         #tmp_value =  (hex_arr[h].hex.to_i - 8) * (power_value.to_i)
	 #tmp_value -= 8 * (power_value.to_i)
	 tmp_value  = (hex_arr[h].hex - 16)  * (power_value.to_i)
      end
  end

   print  'power_value->',power_value,"\n"
	print  'tmp_value->',tmp_value,"\n\n"

  result_i +=  (tmp_value.to_i)
end	


puts "[#{hex_str}] -> [#{result_i}]"