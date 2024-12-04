#pragma once

namespace TheaterManagementSystem {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class SeatingGridForm : public Form // ref class for winforms and .net features
    {
    private:
        array<bool>^ seats; // array of bools for seats availability
        int bookedSeats;    // number of currently booked seats

        // winforms ui elements
        Label^ headerLabel;
        Label^ seatCounterLabel;
        TableLayoutPanel^ seatingGrid;

    public:
        // constructor
        SeatingGridForm(String^ movieTitle, array<bool>^ seats) : seats(seats) // initialize seats with data passed from the MovieListForm
        {
            InitializeComponent(); // setup winforms elements
            this->Text = movieTitle; // movie title at the top of the menu

            bookedSeats = 0;
            for each (bool seat in seats) { // loop through all seats in the array
                if (!seat) ++bookedSeats; // change to false if the seat is booked
            }
            UpdateSeatCounter(); // update gui seat counter
            PopulateSeatingGrid(); // create and populate the seat grid
        }

    protected:
        ~SeatingGridForm() // destructor
        {
            // check components container and clear it
            if (components)
            {
                delete components;
            }
        }

    private:
        System::ComponentModel::Container^ components; // container called 'components' for winforms elements

/* -------------------------------------------------------------------------------------------------------------------- */
#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->Size = System::Drawing::Size(900, 700);
            this->BackColor = Color::White;

            this->headerLabel = gcnew Label();
            this->headerLabel->Text = L"Seating Grid";
            this->headerLabel->Font = gcnew Drawing::Font(L"Segoe UI", 18, FontStyle::Bold);
            this->headerLabel->Location = Point(20, 20);
            this->headerLabel->AutoSize = true;

            this->seatCounterLabel = gcnew Label();
            this->seatCounterLabel->Font = gcnew Drawing::Font(L"Segoe UI", 14);
            this->seatCounterLabel->Location = Point(20, 60);
            this->seatCounterLabel->AutoSize = true;

            this->seatingGrid = gcnew TableLayoutPanel();
            this->seatingGrid->Location = Point(20, 100);
            this->seatingGrid->Size = System::Drawing::Size(850, 500);
            this->seatingGrid->RowCount = 11;
            this->seatingGrid->ColumnCount = 11;
            this->seatingGrid->CellBorderStyle = TableLayoutPanelCellBorderStyle::Single;

            for (int i = 0; i < 11; ++i) {
                this->seatingGrid->RowStyles->Add(gcnew RowStyle(SizeType::Percent, 9.09f));
                this->seatingGrid->ColumnStyles->Add(gcnew ColumnStyle(SizeType::Percent, 9.09f));
            }

            this->Controls->Add(this->headerLabel);
            this->Controls->Add(this->seatCounterLabel);
            this->Controls->Add(this->seatingGrid);
        }

        void OnSeatButtonClicked(Object^ sender, EventArgs^ e)
        {
            Button^ seatButton = safe_cast<Button^>(sender);
            int index = safe_cast<int>(seatButton->Tag);

            if (seatButton->BackColor == Color::Green) {
                if (MessageBox::Show(L"Book this seat?", L"Confirm", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes) {
                    seatButton->BackColor = Color::Red;
                    seats[index] = false;
                    ++bookedSeats;
                }
            }
            else if (seatButton->BackColor == Color::Red) {
                if (MessageBox::Show(L"Refund this seat?", L"Confirm", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == System::Windows::Forms::DialogResult::Yes) {
                    seatButton->BackColor = Color::Green;
                    seats[index] = true;
                    --bookedSeats;
                }
            }

            UpdateSeatCounter();
        }
#pragma endregion
/* -------------------------------------------------------------------------------------------------------------------- */

        // populate the seating grid
        void PopulateSeatingGrid()
        {
            for (int i = 0; i < 11; ++i) { // loop through each row in the grid
                for (int j = 0; j < 11; ++j) { // loop through each column in the grid

                    if (i == 0 && j == 0) { // if we are at the top-left corner of the grid, leave it blank
                        this->seatingGrid->Controls->Add(gcnew Label(), j, i); 
                    }

                    else if (i == 0) { // if seat is in the first row, add column labels (1-10)
                        Label^ colLabel = gcnew Label();
                        colLabel->Text = j.ToString(); // add column number to label
                        colLabel->TextAlign = ContentAlignment::MiddleCenter; // center the label text
                        colLabel->Dock = DockStyle::Fill; // make the label fill the cell
                        this->seatingGrid->Controls->Add(colLabel, j, i); // add the label to the grid
                    }

                    else if (j == 0) { // if seat is in the first column, add row labels (A-J, excluding row 5)
                        Label^ rowLabel = gcnew Label();
                        rowLabel->Text = ((i == 5) ? L"" : ((wchar_t)('A' + (i > 5 ? i - 2 : i - 1))).ToString()); // add row letter to label, skipping 5th row
                        rowLabel->TextAlign = ContentAlignment::MiddleCenter; // center the label text
                        rowLabel->Dock = DockStyle::Fill; // make the label fill the cell
                        this->seatingGrid->Controls->Add(rowLabel, j, i); // add the label to the grid
                    }

                    else if (i == 5) { // if seat is in the 5th row, add blank cells for walkway
                        Label^ walkwayLabel = gcnew Label();
                        walkwayLabel->BackColor = Color::White; // make label blank white
                        walkwayLabel->Dock = DockStyle::Fill; // make the label fill the cell
                        this->seatingGrid->Controls->Add(walkwayLabel, j, i); // add the label to the grid
                    }

                    else { // otherwise, add button for seat
                        int seatIndex = ((i > 5 ? i - 2 : i - 1) * 10 + (j - 1)); // calculate seat index, skipping 5th row
                        Button^ seatButton = gcnew Button(); // create button
                        seatButton->BackColor = seats[seatIndex] ? Color::Green : Color::Red; // green = available, red = booked
                        seatButton->Dock = DockStyle::Fill; // make the button fill the cell
                        seatButton->Tag = seatIndex; // store the seat index in the button
                        seatButton->Click += gcnew EventHandler(this, &SeatingGridForm::OnSeatButtonClicked); // attach click event
                        this->seatingGrid->Controls->Add(seatButton, j, i); // add the button to the grid
                    }
                }
            }
        }

        // display available seats
        void UpdateSeatCounter()
        {
            this->seatCounterLabel->Text = String::Format(L"Available Seats: {0}/100", 100 - bookedSeats);
        }
    };
}
