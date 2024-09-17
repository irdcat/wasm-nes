#include "AudioChannel.hpp"

bool AudioChannel::isEnabled()
{
    return enabled;
}

void AudioChannel::enable(bool enable)
{
    enabled = enable;
}
