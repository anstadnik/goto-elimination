FROM archlinux

# Load necessary tools for installing yay
RUN pacman --noconfirm -Sy archlinux-keyring base-devel git cmake gtest ninja openssh gmock gcovr

# Install necessary tools
RUN git clone https://gitlab.com/astadnik/goto-elimination\
			&& cd goto-elimination\
			&& git clone https://gitlab.com/astadnik/algs lib/algs\
			&& cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja -B build\
			&& ninja -C build\
			&& cd build\
			&& ./tests/test

# Run the application
ENTRYPOINT ["/goto-elimination/build/app"]
