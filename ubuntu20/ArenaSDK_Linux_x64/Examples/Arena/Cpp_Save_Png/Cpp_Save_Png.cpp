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

// Save: Png
//    This example introduces saving PNG image data in the saving library. It
//    shows the construction of an image parameters object and an image writer,
//    sets writer to PNG and saves a single PNG image.

// =-=-=-=-=-=-=-=-=-
// =-=- SETTINGS =-=-
// =-=-=-=-=-=-=-=-=-

// pixel format
#define PIXEL_FORMAT BGR8

// file name
#define FILE_NAME "Images/Cpp_Save/image.png"

// =-=-=-=-=-=-=-=-=-
// =-=- EXAMPLE -=-=-
// =-=-=-=-=-=-=-=-=-

// demonstrates saving an image
// (1) converts image to a displayable pixel format
// (2) prepares image parameters
// (3) prepares image writer
// (4) sets image writer to PNG
// (5) saves image
// (6) destroys converted image
void SaveImage(Arena::IImage* pImage, const char* filename)
{
	// convert image
	std::cout << TAB1 << "Convert image to " << GetPixelFormatName(PIXEL_FORMAT) << "\n";

	auto pConverted = Arena::ImageFactory::Convert(
		pImage,
		PIXEL_FORMAT);

	// prepare image parameters
	std::cout << TAB1 << "Prepare image parameters\n";

	Save::ImageParams params(
		pConverted->GetWidth(),
		pConverted->GetHeight(),
		pConverted->GetBitsPerPixel());

	// prepare image writer
	std::cout << TAB1 << "Prepare image writer\n";

	Save::ImageWriter writer(
		params,
		filename);

	// Set image writer to PNG
	//   Set the output file format of the image writer to PNG.
	//   The writer saves the image file as PNG file even without
	//	 the extension in the file name. Aside from this setting, 
	//   compression level can be set between 0 to 9 and the image
	//   can be created using interlacing by changing the parameters. 

	std::cout << TAB1 << "Set image writer to PNG\n";

	writer.SetPng(".png", 0, false);

	// save image
	std::cout << TAB1 << "Save image\n";

	writer << pConverted->GetData();

	// destroy converted image
	Arena::ImageFactory::Destroy(pConverted);
}

// =-=-=-=-=-=-=-=-=-
// =- PREPARATION -=-
// =- & CLEAN UP =-=-
// =-=-=-=-=-=-=-=-=-

int main()
{
	// flag to track when an exception has been thrown
	bool exceptionThrown = false;

	std::cout << "Cpp_Save_Png\n";

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
