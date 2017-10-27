for file in *.sh
do
  [ $file = "install_driver.sh" ] && continue
  . "./$file"
done
