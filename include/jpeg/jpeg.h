#ifndef JPEG_H
#define JPEG_H
#include"utils/utils.h"
#include"entropyencoding/huffman.h"





constexpr std::uint8_t ZZ[du_vert_size][du_vert_size] ={
	{ 0, 1, 5, 6,14,15,27,28},\
	{ 2, 4, 7,13,16,26,29,42},\
	{ 3, 8,12,17,25,30,41,43},\
	{ 9,11,18,24,31,40,44,53},\
	{10,19,23,32,39,45,52,54},\
	{20,22,33,38,46,51,55,60},\
	{21,34,37,47,50,56,59,61},\
	{35,36,48,49,57,58,62,63}
};
constexpr quantization_table_type QLUM ={
	{16,11,10,16,24 ,40 ,51 ,61},\
	{12,12,14,19,26 ,58 ,60 ,55},\
	{14,13,16,24,40 ,57 ,69 ,56},\
	{14,17,22,29,51 ,87 ,80 ,62},\
	{18,22,37,56,68 ,109,103,77},\
	{24,35,55,64,81 ,104,113,92},\
	{49,64,78,87,103,121,120,101},\
	{72,92,98,98,112,100,103,99}
};
constexpr quantization_table_type QCHR ={
	{17,18,24,47,99,99,99,99},\
	{18,21,26,66,99,99,99,99},\
	{24,26,56,99,99,99,99,99},\
	{47,66,99,99,99,99,99,99},\
	{99,99,99,99,99,99,99,99},\
	{99,99,99,99,99,99,99,99},\
	{99,99,99,99,99,99,99,99},\
	{99,99,99,99,99,99,99,99}
};

struct HuffmanCompTable{
   HuffmanDc dc_;
   HuffmanAc ac_;
};

enum class Component:std::uint8_t{
    Y = 1,
    Cb = 2,
    Cr = 3
};

enum class TableSelect:std::uint8_t{
    DCLUM = 0,
    DCCHR = 1,
    ACLUM = 2,
    ACCHR = 3
};

struct RawImage{
    
    std::size_t num_channels;
    std::size_t vert_size;
    std::size_t hor_size;
    std::size_t channel_size{vert_size*hor_size};
    std::vector<std::uint16_t>payload;
};

struct GroupedImage{
    public:
    GroupedImage(const std::size_t num_v_blocks,
		 const std::size_t num_h_blocks):
	    num_v_blocks{0}
	   ,num_h_blocks{0}
	   ,data_(num_h_blocks*num_v_blocks)
    {}

    DataUnit& operator()(std::size_t v,std::size_t h){
        return data_[v*num_h_blocks+h];
    }
    std::size_t num_v_blocks{0};
    std::size_t num_h_blocks{0};
    std::vector<DataUnit>data_;
};
    struct FrameHeader{
    	/********** Frame Header ***********/
    	///\brief Value corresponds to index: SOF_{index}
        std::uint16_t SOF;
    	///\brief Frame header length.
	std::uint16_t Lf;
    	///\brief Sample precision.
	std::uint8_t P;
    	///\brief Number of lines in the source image.
	std::uint16_t Y;
    	///\brief Number of samples per line in the source image.
    	std::uint16_t X;
    	///\brief Number of components per frame.
	std::uint8_t Nf;
    	///\brief Component specifier.
    	std::vector<std::uint8_t> Ci;
    	///\brief Horizontal and vertical sampling factor of component C (4 bits each).
    	std::vector<uint8_t> HV;
    	///\brief Quantization table identifier of component C.
    	std::vector<std::uint8_t> Tq;
    };
    struct ScanHeader{
    	/********** Scan Header ************/
    	///\brief Start of scan marker
        std::uint16_t SOS;
    	///\brief Scan header length
	std::uint16_t Ls;
    	///\brief Number of components in scan. 
	std::uint8_t Ns;
    	///\brief Scan component selector. 
	std::vector<std::uint8_t> Cs;
    	///\brief DCAC entropy coding table destination selector. 
	std::vector<std::uint8_t> Tda;
    	///\brief SS Start of spectral or predictor selection. 
	std::uint8_t Ss;
    	///\brief SS End of spectral or predictor selection. 
	std::uint8_t Se;
    	///\brief Ah Successive approximation bit position high.j 
	std::uint8_t AhAl;
    	/********** Scan Header ************/
    	/// Define quantization table marker
    };
    struct QTableHeader{
	///\brief Start of quantization table marker
	std::uint16_t DQT;
    	/// Quantization table definition length
    	std::uint16_t Lq;
	    struct QTable{
    	    /// Quantization table element precision 
	    std::uint8_t PqTq;
    	    /// Quantization table element 
	    const quantization_table_type*ptr_quantization_table_;
	};
	    std::vector<QTable> qtables_;
    };
  //TODO: Make template param a concept. 
    struct HTableHeader{
	///\brief Start of huffman table marker
        std::uint16_t DHT;
    	/// Huffman table definition length 
	std::uint16_t Lh;
	struct HTable{
    	    /// Table class (for dc table)
	    std::uint8_t dc_TcTh;
    	    /// Table class (for ac table)
	    std::uint8_t ac_TcTh;
	    /// Pointer to huffman tables (TODO: weak_ptr?)
	    HuffmanCompTable*ptr_huffman_table_;
	};
        std::vector<HTable> htables_;
    };
    struct JpegMetaData{
        FrameHeader fh_;
        std::vector<ScanHeader> sh_; //TODO: Perhaps, there are multiple scan headers...
        QTableHeader qth_; //Quantization tables
        HTableHeader hth_; //Huffman tables
			      //
    };



class JpegEncoder{
    private:
    JpegMetaData jpeg_meta_data_;


    std::vector<HuffmanCompTable> huffman_tables_;

    JpegRaw jpeg_raw_;

    std::size_t num_bits_written_;


    public:
    JpegEncoder();

    JpegRaw Encode(RawImage&raw);

    void Encode(DataUnit& data_unit);
    void Encode(std::size_t R,std::int32_t ZZ_K,EHUFSI&,EHUFCO&);

    template<typename T>
    void appendBytes(const T bytes);

    template<typename T>
    void appendBits(const T bits,std::size_t size);

    void writeData(GroupedImage&gimage);
    void writeStartOfImage();
    void writeEndOfImage();
    void writeFrameHeader();
    void writeHuffmanTables();
    void writeQuantizationTables();
    void writeQuantizationTable();

    void writeScans(GroupedImage&gimage);
    void writeScanHeader();

    void EntropyEncoding(GroupedImage&gimage);


    void computeHuffmanTable(RawImage&raw);


    GroupedImage createGroupedImageFromRaw(RawImage&raw);


    HuffmanDc&getHuffmanDcLum(){
        return huffman_tables_[0].dc_;
    }
    HuffmanDc&getHuffmanDcChr(){
        return huffman_tables_[1].dc_;
    }
    HuffmanAc&getHuffmanAcLum(){
        return huffman_tables_[0].ac_;
    }
    HuffmanAc&getHuffmanAcChr(){
        return huffman_tables_[1].ac_;
    }

    void printRawJpeg();

};

#endif
