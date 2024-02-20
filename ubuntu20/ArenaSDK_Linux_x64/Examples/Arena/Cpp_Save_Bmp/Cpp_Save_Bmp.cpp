/***************************************************************************************
 ***                                                                                 ***
 ***  Copyright (c) 2023, Lucid Vision Labs, Inc.                                    ***
 ***                                                                                 ***
 ***  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     ***
 ***  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       ***
 ***  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    ***
 ***  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         ***
 ***  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  ***
 ***  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  ***
 ***  SOFTWARE.                                                                      ***
 ***                                                                                 ***
 ***************************************************************************************/

#include "stdafx.h"
#include "ArenaApi.h"
#include "SaveApi.h"

#define TAB1 "  "

// Save: Bmp
//    This example introduces saving BMP image data in the saving library. It
//    shows the construction of an image parameters object and an image writer,
//    sets writer to BMP and saves a single BMP image.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// pixel format
#define PIXEL_FORMAT BGR8

// file name
#define FILE_NAME "Images/Cpp_Save/image.bmp"

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates saving an image
// (1) prepares image parameters
// (2) prepares image writer
// (3) sets image writer to BMP
// (4) saves image
void SaveImage(Arena::IImage* pImage, const char* filename)
{
	// prepare image parameters
	std::cout << TAB1 << "Prepare image parameters\n";

	Save::ImageParams params(
		pImage->GetWidth(),
		pImage->GetHeight(),
		pImage->GetBitsPerPixel());

	// prepare image writer
	std::cout << TAB1 << "Prepare image writer\n";

	Save::ImageWriter writer(
		params,
		filename);

	// Set image writer to BMP
	//   Set the output file format of the image writer to BMP.
	//   The writer saves the image file as BMP file even without
	//	 the extension in the file name. Aside from this setting,
	//   compression can be set several different compression algorithms, 
	//   and store tags for separated CMYK by changing the parameters.
	std::cout << TAB1 << "Set image writer to BMP\n";

	writer.SetBmp(".bmp");
			
	// save image
	std::cout << TAB1 << "Save image\n";

	writer << pImage->GetData();
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Save_Bmp\n";

	try
	{
		// prepare example
		Arena::ISystem* pSystem = Arena::OpenSystem();
		pSystem->UpdateDevices(100);
		std::vector<Arena::DeviceInfo> devices = pSystem->GetDevices();
		if (devices.size() == 0)
		{
			std::cout << "\nNo camera connected\nPress enter to complete\n";
			std::getchar();
			return 0;
		}
		Arena::IDevice* pDevice = pSystem->CreateDevice(devices[0]);

		// enable stream auto negotiate packet size
		Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamAutoNegotiatePacketSize", true);

		// enable stream packet resend
		Arena::SetNodeValue<bool>(pDevice->GetTLStreamNodeMap(), "StreamPacketResendEnable", true);

		pDevice->StartStream();
		Arena::IImage* pImage = pDevice->GetImage(2000);

		std::cout << "Commence example\n\n";
		SaveImage(pImage, FILE_NAME);
		std::cout << "\nExample complete\n";

		// clean up example
		pDevice->RequeueBuffer(pImage);
		pDevice->StopStream();
		pSystem->DestroyDevice(pDevice);
		Arena::CloseSystem(pSystem);
	}
	catch (GenICam::GenericException& ge)
	{
		std::cout << "\nGenICam exception thrown: " << ge.what() << "\n";
		exceptionThrown = true;
	}
	catch (std::exception& ex)
	{
		std::cout << "\nStandard exception thrown: " << ex.what() << "\n";
		exceptionThrown = true;
	}
	catch (...)
	{
		std::cout << "\nUnexpected exception thrown\n";
		exceptionThrown = true;
	}

	std::cout << "Press enter to complete\n";
	std::getchar();

	if (exceptionThrown)
		return -1;
	else
		return 0;
}
