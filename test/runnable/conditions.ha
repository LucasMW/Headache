void main() {
	byte a;
	a = 10;
	if(a>1) { 
		if(a>2){ 
			if(a>3) { 
				if(a>4){ 
					if(a>5){ 
						@a;@" is greater than five\n";
					}
					@a;@" is greater than four\n";
				}
				@a;@" is greater than three\n";
			}
			@a;@" is greater than two\n";
		} 
		@a;@" is greater than one\n";

	} else if(true) {
		@"Block 1";@"\n";
	} else if(2==2) {
		@"Block 2";@"\n";
	} else if(3<4) {
		@"Block 3";@"\n";
	}
}