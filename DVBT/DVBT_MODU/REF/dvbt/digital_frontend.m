%% digital_frontend Digital frontend for DVB-T reciever.
%%
%%   y = digital_frontend (x) performs digital frontend filtering for
%%   the DVB-T reciever. It performs gain control, synchronization,
%%   and sample rate conversion if applicable.

%%---------------------------------------------------------------------------
%%
%%     (C) COPYRIGHT 2003 VODAFONE Chair Mobile Communications Systems,
%%                    Technische Universitšt Dresden
%%                          ALL RIGHTS RESERVED
%%
%%---------------------------------------------------------------------------
%% Title         : Digital Frontend
%% Project       : MOUSE
%%
%% File          : digital_frontend.m
%% Author        : Gordon Cichon <cichon@radionetworkprocessor.com>
%%
%% Created       : 2003/04/23
%% Last checkin  : $Date: 2003/12/08 19:29:43 $ by $Author: gordon $
%% Revision      : $Revision: 1.3 $
%%---------------------------------------------------------------------------

function output = digital_frontend (input)

  global VISUALIZATION;
  
  %% Visualization
  if VISUALIZATION.signal 
    figure(VISUALIZATION.figure.signal);
    %%gset title "channel signal";
    title('real');
    plot(real(input),'-');
    title('imag');
    plot(imag(input),'-');
  end

  input = gain_control (input);
  synchronization (input);

  output = sample_rate_conversion (input);
