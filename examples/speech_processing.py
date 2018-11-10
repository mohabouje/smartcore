from argparse import ArgumentParser
from time import sleep
from pyaudio import PyAudio, paFloat32, paContinue
from smartcore.dsp import cepstrum, periodogram, preemphasis, windower, detrend

dc = detrend.DeTrend()
pe = preemphasis.PreEmphasis()
win = windower.Window()
ceps = cepstrum.Cepstrum()
spec = periodogram.Periodogram()


def audio_processing(in_data, frame_count, time_info, flag):
    in_data = dc.run(in_data)
    in_data = pe.run(in_data)
    in_data = win.run(in_data)
    cepstral = ceps.run(in_data)
    spectral = spec.run(in_data)
    return in_data, paContinue


def main():

    parser = ArgumentParser(description="Process the microphone input in real time")
    parser.add_argument("fs", type=float, nargs='?', default=8000, help="Sampling rate in Hz")
    parser.add_argument("channels", type=int, nargs='?',  default=1, help="Number of channels")
    parser.add_argument("size", type=int, nargs='?', default=1024, help="Number of frames per buffer")
    parser.add_argument("index", type=int, nargs='?', default=0, help="Number of frames per buffer")
    args = parser.parse_args()

    pa = PyAudio()
    stream = pa.open(format=paFloat32, channels=args.channels, frames_per_buffer=args.size,
                     rate=args.fs, input=True, stream_callback=audio_processing)

    while stream.is_active():
        sleep(0.1)

    stream.close()
    pa.terminate()


if __name__ == "__main__":
    main()
