module ESP32
  class File < IO
    attr_accessor :path  
    def initialize(fd_or_path, mode = "r", perm = 0666)
      if fd_or_path.kind_of? Fixnum
        super(fd_or_path, mode)
      else
        @path = fd_or_path
        fd = IO.sysopen(@path, mode, perm)
        super(fd, mode)
      end
    end
  end
end
